/* Copyright (c) 2014, 2023, Oracle and/or its affiliates.

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

#ifndef DD_TABLES__COLUMN_TYPE_ELEMENTS_INCLUDED
#define DD_TABLES__COLUMN_TYPE_ELEMENTS_INCLUDED

#include "sql/dd/impl/types/object_table_impl.h"  // dd::Object_table_impl
#include "sql/dd/object_id.h"                     // dd::Object_id
#include "sql/dd/string_type.h"

namespace dd {
class Object_key;

namespace tables {

///////////////////////////////////////////////////////////////////////////

class Column_type_elements : public Object_table_impl {
 public:
  static const Column_type_elements &instance();

  enum enum_fields {
    FIELD_COLUMN_ID,
    FIELD_ELEMENT_INDEX,
    FIELD_NAME,
    NUMBER_OF_FIELDS  // Always keep this entry at the end of the enum
  };

  enum enum_indexes { INDEX_PK_COLUMN_ID_ELEMENT_INDEX };

  enum enum_foreign_keys { FK_COLUMN_ID };

  Column_type_elements();

  static Object_key *create_key_by_column_id(Object_id column_id);

  static Object_key *create_primary_key(Object_id column_id, int index);
};

///////////////////////////////////////////////////////////////////////////

}  // namespace tables
}  // namespace dd

#endif  // DD_TABLES__COLUMN_TYPE_ELEMENTS_INCLUDED
