/*
  Copyright (c) 2015, 2023, Oracle and/or its affiliates.

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
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ifndef STORED_PROCEDURE_INCLUDED
#define STORED_PROCEDURE_INCLUDED

#include <string>

#include "client/dump/abstract_plain_sql_object_dump_task.h"
#include "my_inttypes.h"

namespace Mysql {
namespace Tools {
namespace Dump {

class Stored_procedure : public Abstract_plain_sql_object_dump_task {
 public:
  Stored_procedure(uint64 id, const std::string &name,
                   const std::string &schema,
                   const std::string &sql_formatted_definition);
};

}  // namespace Dump
}  // namespace Tools
}  // namespace Mysql

#endif
