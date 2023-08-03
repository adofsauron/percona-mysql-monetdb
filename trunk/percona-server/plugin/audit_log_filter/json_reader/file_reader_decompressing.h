/* Copyright (c) 2023 Percona LLC and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */

#ifndef AUDIT_LOG_FILTER_JSON_READER_FILE_READER_DECOMPRESSING_H_INCLUDED
#define AUDIT_LOG_FILTER_JSON_READER_FILE_READER_DECOMPRESSING_H_INCLUDED

#include "plugin/audit_log_filter/json_reader/file_reader_decorator_base.h"

#include "zlib.h"

namespace audit_log_filter::json_reader {

class FileReaderDecompressing final : public FileReaderDecoratorBase {
  static inline size_t const kInBufferSize = 32768;

 public:
  explicit FileReaderDecompressing(std::unique_ptr<FileReaderBase> file_reader);

  ~FileReaderDecompressing() override;

  bool init() noexcept override;
  bool open(FileInfo *file_info) noexcept override;
  void close() noexcept override;
  ReadStatus read(unsigned char *out_buffer, size_t out_buffer_size,
                  size_t *read_size) noexcept override;

 private:
  z_stream m_strm{};
  unsigned char m_in_buff[kInBufferSize] = {0};
  bool is_opened = false;
};

}  // namespace audit_log_filter::json_reader

#endif  // AUDIT_LOG_FILTER_JSON_READER_FILE_READER_DECOMPRESSING_H_INCLUDED