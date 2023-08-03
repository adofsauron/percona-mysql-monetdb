/* Copyright (c) 2018, 2023, Oracle and/or its affiliates.

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

#ifndef PLUGIN_X_SRC_SERVICES_MYSQLX_GROUP_MEMBERSHIP_LISTENER_H_
#define PLUGIN_X_SRC_SERVICES_MYSQLX_GROUP_MEMBERSHIP_LISTENER_H_

#include <mysql/components/service_implementation.h>
#include <mysql/components/services/group_membership_listener.h>

extern SERVICE_TYPE_NO_CONST(group_membership_listener)
    SERVICE_IMPLEMENTATION(mysqlx, group_membership_listener);

#endif  // PLUGIN_X_SRC_SERVICES_MYSQLX_GROUP_MEMBERSHIP_LISTENER_H_
