/* -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*- */
// vim: ft=cpp:expandtab:ts=8:sw=4:softtabstop=4:
#ident "$Id$"
/*======
This file is part of PerconaFT.


Copyright (c) 2006, 2015, Percona and/or its affiliates. All rights reserved.

    PerconaFT is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License, version 2,
    as published by the Free Software Foundation.

    PerconaFT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PerconaFT.  If not, see <http://www.gnu.org/licenses/>.

----------------------------------------

    PerconaFT is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License, version 3,
    as published by the Free Software Foundation.

    PerconaFT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with PerconaFT.  If not, see <http://www.gnu.org/licenses/>.
======= */

#ident "Copyright (c) 2006, 2015, Percona and/or its affiliates. All rights reserved."

#pragma once

#include <dirent.h>
#include <sys/time.h>

#include "toku_stdint.h"
#include "toku_os_types.h"

// Returns: the current process id
int toku_os_getpid(void)   __attribute__((__visibility__("default")));

// Returns: the current thread id
int toku_os_gettid(void)  __attribute__((__visibility__("default")));

// Returns: the number of processors in the system
int toku_os_get_number_processors(void);

// Returns: the number of active processors in the system
int toku_os_get_number_active_processors(void);

// Returns: the system page size (in bytes)
int toku_os_get_pagesize(void);

// Returns: the size of physical memory (in bytes)
uint64_t toku_os_get_phys_memory_size(void) __attribute__((__visibility__("default")));

// Returns the processor frequency in Hz
// Returns 0 if success
int toku_os_get_processor_frequency(uint64_t *hz);

// Returns: 0 on success
// sets fsize to the number of bytes in a file
int toku_os_get_file_size(int fildes, int64_t *fsize)   __attribute__((__visibility__("default")));

// Returns: 0 on success
// Initializes id as a unique fileid for fildes on success.
int toku_os_get_unique_file_id(int fildes, struct fileid *id);

//Locks a file (should not be open to begin with).
//Returns: file descriptor (or -1 on error)
int toku_os_lock_file(const char *name);

//Unlocks and closes a file locked by toku_os_lock_on_file
int toku_os_unlock_file(int fildes);

int toku_os_mkdir(const char *pathname, mode_t mode) __attribute__((__visibility__("default")));

// Get the current process user and kernel use times
int toku_os_get_process_times(struct timeval *usertime, struct timeval *kerneltime);

// Get the maximum size of the process data size (in bytes)
// Success: returns 0 and sets *maxdata to the data size
// Fail: returns an error number
int toku_os_get_max_process_data_size(uint64_t *maxdata) __attribute__((__visibility__("default")));

int toku_os_initialize_settings(int verbosity)  __attribute__((__visibility__("default")));

bool toku_os_is_absolute_name(const char* path)  __attribute__((__visibility__("default")));

// Return true if huge pages are enabled. See portability/huge_page_detection.cc for methodology.
bool toku_os_huge_pages_enabled(void) __attribute__((__visibility__("default")));

// Set whether or not writes assert when ENOSPC is returned or they wait for space
void toku_set_assert_on_write_enospc(int do_assert) __attribute__((__visibility__("default")));

// Get file system write information
// *enospc_last_time is the last time ENOSPC was returned by write or pwrite
// *enospc_current   is the number of threads waiting on space
// *enospc_total     is the number of times ENOSPC was returned by write or pwrite
void toku_fs_get_write_info(time_t *enospc_last_time, uint64_t *enospc_current, uint64_t *enospc_total);

void toku_fsync_dirfd_without_accounting(DIR *dirp);

int toku_fsync_dir_by_name_without_accounting(const char *dir_name);

// Get the file system free and total space for the file system that contains a path name
// *avail_size is set to the bytes of free space in the file system available for non-root 
// *free_size is set to the bytes of free space in the file system
// *total_size is set to the total bytes in the file system
// Return 0 on success, otherwise an error number
int toku_get_filesystem_sizes(const char *path,
                              uint64_t *avail_size,
                              uint64_t *free_size,
                              uint64_t *total_size);

// Portable linux 'dup2'
int toku_dup2(int fd, int fd2) __attribute__((__visibility__("default")));
