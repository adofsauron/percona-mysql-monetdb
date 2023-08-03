/* Copyright (c) 2011, 2023, Oracle and/or its affiliates.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

   This program is also distributed with certain software (including
   but not limited to OpenSSL) that is licensed under separate terms,
   as designated in a particular file or component or in included license
   documentation.  The authors of MySQL hereby grant you an additional
   permission to link the program and your derivative works with the
   separately licensed software that they have included with MySQL.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License, version 2.0, for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

#include "vio_priv.h"

#include "my_dbug.h"

static size_t wait_overlapped_result(Vio *vio, int timeout) {
  size_t ret = (size_t)-1;
  DWORD transferred, wait_status, timeout_ms;

  timeout_ms = timeout >= 0 ? timeout : INFINITE;

  /* Wait for the overlapped operation to be completed. */
  wait_status = WaitForSingleObjectEx(vio->overlapped.hEvent, timeout_ms, TRUE);

  /* The operation might have completed, attempt to retrieve the result. */
  if (wait_status == WAIT_OBJECT_0) {
    /* If retrieval fails, a error code will have been set. */
    if (GetOverlappedResult(vio->hPipe, &vio->overlapped, &transferred, false))
      ret = transferred;
  } else {
    /* Error or timeout, cancel the pending I/O operation. */
    CancelIo(vio->hPipe);

    /*
      If the wait timed out, set error code to indicate a
      timeout error. Otherwise, wait_status is WAIT_FAILED
      and extended error information was already set.
    */
    if (wait_status == WAIT_TIMEOUT) SetLastError(SOCKET_ETIMEDOUT);
  }

  return ret;
}

/*
  Disable posting IO completion event to the port.
  In some cases (synchronous timed IO) we want to skip IOCP notifications.
*/
static void disable_iocp_notification(OVERLAPPED *overlapped) {
  HANDLE *handle = &(overlapped->hEvent);
  *handle = ((HANDLE)((ULONG_PTR)*handle | 1));
}

/* Enable posting IO completion event to the port */
static void enable_iocp_notification(OVERLAPPED *overlapped) {
  HANDLE *handle = &(overlapped->hEvent);
  *handle = (HANDLE)((ULONG_PTR)*handle & ~1);
}

size_t vio_read_pipe(Vio *vio, uchar *buf, size_t count) {
  DWORD transferred;
  size_t ret = (size_t)-1;
  DBUG_TRACE;

  disable_iocp_notification(&vio->pipe_overlapped);

  /* Attempt to read from the pipe (overlapped I/O). */
  if (ReadFile(vio->hPipe, buf, (DWORD)count, &transferred, &vio->overlapped)) {
    /* The operation completed immediately. */
    ret = transferred;
  }
  /* Read operation is pending completion asynchronously? */
  else if (GetLastError() == ERROR_IO_PENDING)
    ret = wait_overlapped_result(vio, vio->read_timeout);

  enable_iocp_notification(&vio->pipe_overlapped);

  return ret;
}

size_t vio_write_pipe(Vio *vio, const uchar *buf, size_t count) {
  DWORD transferred;
  size_t ret = (size_t)-1;
  DBUG_TRACE;

  disable_iocp_notification(&vio->pipe_overlapped);

  /* Attempt to write to the pipe (overlapped I/O). */
  if (WriteFile(vio->hPipe, buf, (DWORD)count, &transferred,
                &vio->overlapped)) {
    /* The operation completed immediately. */
    ret = transferred;
  }
  /* Write operation is pending completion asynchronously? */
  else if (GetLastError() == ERROR_IO_PENDING)
    ret = wait_overlapped_result(vio, vio->write_timeout);

  enable_iocp_notification(&vio->pipe_overlapped);

  return ret;
}

bool vio_is_connected_pipe(Vio *vio) {
  if (PeekNamedPipe(vio->hPipe, NULL, 0, NULL, NULL, NULL)) return true;
  return (GetLastError() != ERROR_BROKEN_PIPE);
}

int vio_shutdown_pipe(Vio *vio, int how) {
  BOOL ret = FALSE;
  DBUG_TRACE;

  if (vio->inactive == false) {
    CancelIo(vio->hPipe);
    CloseHandle(vio->overlapped.hEvent);
    FlushFileBuffers(vio->hPipe);
    DisconnectNamedPipe(vio->hPipe);
    ret = CloseHandle(vio->hPipe);
  }

  vio->inactive = true;
  vio->hPipe = NULL;
  vio->mysql_socket = MYSQL_INVALID_SOCKET;

  return ret;
}

int vio_cancel_pipe(Vio *vio, int how) {
  DBUG_ENTER("vio_shutdown_pipe");

  CancelIo(vio->hPipe);
  CloseHandle(vio->overlapped.hEvent);
  DisconnectNamedPipe(vio->hPipe);

  vio->inactive = TRUE;

  DBUG_RETURN(0);
}

#endif
