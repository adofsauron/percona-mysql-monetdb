#ifndef MONETDB_RESULT_CONVERT_FIELD
#define MONETDB_RESULT_CONVERT_FIELD

#include "mem_root_deque.h"
#include "monetdbe.h"
#include "my_compiler.h"
#include "my_dbug.h"
#include "mysql/components/services/dynamic_privilege.h"
#include "mysql/components/services/udf_registration.h"
#include "mysql/plugin.h"
#include "mysql/service_plugin_registry.h"
#include "sql/field.h"
#include "sql/item.h"
#include "sql/item_sum.h"
#include "sql/mysqld.h"
#include "sql/query_result.h"
#include "sql/sql_class.h"
#include "sql/visible_fields.h"

#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wreturn-type"

template <enum_field_types>
char *monetdb_result_convert_field(Field *f, monetdbe_column *rcol, int64_t r) {
  return NULL;
}

template <>
char *monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_STRING>(
    Field *f, monetdbe_column *rcol, int64_t r) {
  if (rcol->type != monetdbe_str) {
    DBUG_PRINT("mdb", ("ERROR: mdb col type:%d not str", rcol->type));
    return "mdb type not str";
  }

  monetdbe_column_str *col = (monetdbe_column_str *)rcol;
  bool is_null = col->is_null(col->data + r) != 0;
  if (!is_null) {
    type_conversion_status status =
        f->store(col->data[r], strlen(col->data[r]), f->charset());
    if (type_conversion_status::TYPE_OK != status) {
      DBUG_PRINT("mdb",
                 ("ERROR: field store fail for str, status: %d", status));
      return "field store fail for str";
    }
  }
  is_null ? f->set_null() : f->set_notnull();
  return NULL;
}

template <>
char *monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_INT24>(
    Field *f, monetdbe_column *rcol, int64_t r) {
  if (rcol->type != monetdbe_int32_t) {
    DBUG_PRINT("mdb", ("ERROR: mdb col type:%d not int32", rcol->type));
    return "mdb type not int32";
  }

  monetdbe_column_int32_t *col = (monetdbe_column_int32_t *)rcol;
  bool is_null = col->data[r] == col->null_value;
  if (!is_null) {
    type_conversion_status status = f->store(col->data[r]);
    if (type_conversion_status::TYPE_OK != status) {
      DBUG_PRINT("mdb",
                 ("ERROR: field store fail for int32, status: %d", status));
      return "field store fail for int32";
    }
  }
  is_null ? f->set_null() : f->set_notnull();
  return NULL;
}

template <>
char *monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_BLOB>(
    Field *f, monetdbe_column *rcol, int64_t r) {
  if (rcol->type != monetdbe_blob) {
    DBUG_PRINT("mdb", ("ERROR: mdb col type:%d not blob", rcol->type));
    return "mdb type not blob";
  }

  monetdbe_column_blob *col = (monetdbe_column_blob *)rcol;
  bool is_null = col->is_null(col->data + r) != 0;
  if (!is_null) {
    type_conversion_status status =
        f->store(col->data[r].data, col->data[r].size, f->charset());
    if (type_conversion_status::TYPE_OK != status) {
      DBUG_PRINT("mdb",
                 ("ERROR: field store fail for blob, status: %d", status));
      return "field store fail for blob";
    }
  }
  is_null ? f->set_null() : f->set_notnull();
  return NULL;
}

template <>
char *monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_LONGLONG>(
    Field *f, monetdbe_column *rcol, int64_t r) {
  if (rcol->type != monetdbe_int64_t) {
    DBUG_PRINT("mdb", ("ERROR: mdb col type:%d not int64", rcol->type));
    return "mdb type not int64";
  }

  monetdbe_column_int64_t *col = (monetdbe_column_int64_t *)rcol;
  bool is_null = col->data[r] == col->null_value;
  if (!is_null) {
    if (type_conversion_status::TYPE_OK !=
        f->store(col->data[r], f->is_unsigned())) {
      return "file store fail for int64";
    }
  }
  is_null ? f->set_null() : f->set_notnull();
  return NULL;
}

template <>
char *monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_DATE>(
    Field *f, monetdbe_column *rcol, int64_t r) {
  if (rcol->type != monetdbe_date) {
    DBUG_PRINT("mdb", ("ERROR: mdb col type:%d not date", rcol->type));
    return "mdb type not date";
  }

  monetdbe_column_date *col = (monetdbe_column_date *)rcol;
  bool is_null = col->is_null(col->data + r) != 0;
  if (!is_null) {
    MYSQL_TIME my_date;
    auto mdb_date = col->data + r;
    my_date.year = static_cast<uint>(mdb_date->year);
    my_date.month = static_cast<uint>(mdb_date->month);
    my_date.day = static_cast<uint>(mdb_date->day);
    my_date.hour = my_date.minute = my_date.second = 0;
    my_date.second_part = 0;
    my_date.neg = false;
    my_date.time_type = MYSQL_TIMESTAMP_DATE;

    if (type_conversion_status::TYPE_OK != f->store_time(&my_date)) {
      return "file store fail for date";
    }
  }
  is_null ? f->set_null() : f->set_notnull();
  return NULL;
}

template <>
char *monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_TIME>(
    Field *f, monetdbe_column *rcol, int64_t r) {
  if (rcol->type != monetdbe_time) {
    DBUG_PRINT("mdb", ("ERROR: mdb col type:%d not time", rcol->type));
    return "mdb type not time";
  }

  monetdbe_column_time *col = (monetdbe_column_time *)rcol;
  bool is_null = col->is_null(col->data + r) != 0;
  if (!is_null) {
    MYSQL_TIME my_time;
    auto mdb_time = col->data + r;
    my_time.hour = static_cast<uint>(mdb_time->hours);
    my_time.minute = static_cast<uint>(mdb_time->minutes);
    my_time.second = static_cast<uint>(mdb_time->seconds);
    /*
      In monetdb, microseconds precision is only 3;
      while in mysql, it is up to 6
    */
    my_time.second_part = static_cast<ulong>(mdb_time->ms) * 1000UL;
    my_time.year = my_time.month = my_time.day = 0;
    my_time.neg = false;
    my_time.time_type = MYSQL_TIMESTAMP_TIME;

    if (type_conversion_status::TYPE_OK !=
        f->store_time(&my_time, f->decimals())) {
      return "file store fail for time";
    }
  }
  is_null ? f->set_null() : f->set_notnull();
  return NULL;
}

template <>
char *monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_TIMESTAMP>(
    Field *f, monetdbe_column *rcol, int64_t r) {
  if (rcol->type != monetdbe_timestamp) {
    DBUG_PRINT("mdb", ("ERROR: mdb col type:%d not timestamp", rcol->type));
    return "mdb type not timestamp";
  }

  monetdbe_column_timestamp *col = (monetdbe_column_timestamp *)rcol;
  bool is_null = col->is_null(col->data + r) != 0;
  if (!is_null) {
    MYSQL_TIME my_ts;
    auto mdb_ts = col->data + r;
    auto mdb_ts_date = &mdb_ts->date;
    auto mdb_ts_time = &mdb_ts->time;
    my_ts.year = static_cast<uint>(mdb_ts_date->year);
    my_ts.month = static_cast<uint>(mdb_ts_date->month);
    my_ts.day = static_cast<uint>(mdb_ts_date->day);
    my_ts.hour = static_cast<uint>(mdb_ts_time->hours);
    my_ts.minute = static_cast<uint>(mdb_ts_time->minutes);
    my_ts.second = static_cast<uint>(mdb_ts_time->seconds);
    /*
     * In monetdb, microseconds precision is only 3;
     * while in mysql, it is up to 6
     */
    my_ts.second_part = static_cast<ulong>(mdb_ts_time->ms) * 1000UL;
    my_ts.neg = false;
    my_ts.time_type = MYSQL_TIMESTAMP_DATETIME;

    if (type_conversion_status::TYPE_OK !=
        f->store_time(&my_ts, f->decimals())) {
      return "file store fail for timestamp";
    }
  }
  is_null ? f->set_null() : f->set_notnull();
  return NULL;
}

template <>
char *monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_TINY>(
    Field *f, monetdbe_column *rcol, int64_t r) {
  if (rcol->type != monetdbe_bool && rcol->type != monetdbe_int8_t) {
    DBUG_PRINT("mdb",
               ("ERROR: mdb col type:%d not bool or tinyint", rcol->type));
    return "mdb type not bool or tinyint";
  }

  // treat bool as int8
  monetdbe_column_int8_t *col = (monetdbe_column_int8_t *)rcol;
  bool is_null = col->data[r] == col->null_value;
  if (!is_null) {
    type_conversion_status status = f->store(col->data[r], f->is_unsigned());
    if (type_conversion_status::TYPE_OK != status) {
      DBUG_PRINT(
          "mdb",
          ("ERROR: field store fail for bool or tinyint, status: %d", status));
      return "field store fail for bool or tinyint";
    }
  }
  is_null ? f->set_null() : f->set_notnull();
  return NULL;
}

template <>
char *monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_JSON>(
    Field *f, monetdbe_column *rcol, int64_t r) {
  if (rcol->type != monetdbe_str) {
    DBUG_PRINT("mdb", ("ERROR: mdb col type:%d not json", rcol->type));
    return "mdb type not json";
  }

  monetdbe_column_str *col = (monetdbe_column_str *)rcol;
  bool is_null = col->is_null(col->data + r) != 0;
  if (!is_null) {
    type_conversion_status status =
        f->store(col->data[r], strlen(col->data[r]), f->charset());
    if (type_conversion_status::TYPE_OK != status) {
      DBUG_PRINT("mdb",
                 ("ERROR: field store fail for json, status: %d", status));
      return "field store fail for json";
    }
  }
  is_null ? f->set_null() : f->set_notnull();
  return NULL;
}

template <>
char *monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_FLOAT>(
    Field *f, monetdbe_column *rcol, int64_t r) {
  bool is_null;
  double result_value;

  if (rcol->type == monetdbe_double) {
    monetdbe_column_double *col = (monetdbe_column_double *)rcol;
    is_null = col->data[r] == col->null_value;
    result_value = is_null ? 0 : col->data[r];
  } else if (rcol->type == monetdbe_float) {
    monetdbe_column_float *col = (monetdbe_column_float *)rcol;
    is_null = col->data[r] == col->null_value;
    result_value = is_null ? 0 : col->data[r];
  } else {
    DBUG_PRINT("mdb", ("ERROR: mdb col type:%d not double", rcol->type));
    return "mdb type not double";
  }

  if (!is_null) {
    if (type_conversion_status::TYPE_OK != f->store(result_value)) {
      return "file store fail for double";
    }
  }
  is_null ? f->set_null() : f->set_notnull();
  return NULL;
}

template <>
char *monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_DOUBLE>(
    Field *f, monetdbe_column *rcol, int64_t r) {
  if (rcol->type != monetdbe_double) {
    DBUG_PRINT("mdb", ("ERROR: mdb col type:%d not double", rcol->type));
    return "mdb type not double";
  }

  monetdbe_column_double *col = (monetdbe_column_double *)rcol;
  bool is_null = col->data[r] == col->null_value;
  if (!is_null) {
    if (type_conversion_status::TYPE_OK != f->store(col->data[r])) {
      return "file store fail for double";
    }
  }
  is_null ? f->set_null() : f->set_notnull();
  return NULL;
}

bool convert_int8_field_to_double(Field *f, monetdbe_column *rcol, int64_t r,
                                  double *res) {
  monetdbe_column_int8_t *col = (monetdbe_column_int8_t *)rcol;
  bool is_null = col->data[r] == col->null_value;
  if (!is_null) {
    __int8_t ori_data = col->data[r], big_base = 1;
    uint dec = f->decimals();
    for (uint i = 0; i < dec; ++i, big_base *= 10)
      ;
    *res = (double)ori_data / big_base;
    return false;
  }
  return true;
}

bool convert_int16_field_to_double(Field *f, monetdbe_column *rcol, int64_t r,
                                   double *res) {
  monetdbe_column_int16_t *col = (monetdbe_column_int16_t *)rcol;
  bool is_null = col->data[r] == col->null_value;
  if (!is_null) {
    __int16_t ori_data = col->data[r], big_base = 1;
    uint dec = f->decimals();
    for (uint i = 0; i < dec; ++i, big_base *= 10)
      ;
    *res = (double)ori_data / big_base;
    return false;
  }
  return true;
}

bool convert_int32_field_to_double(Field *f, monetdbe_column *rcol, int64_t r,
                                   double *res) {
  monetdbe_column_int32_t *col = (monetdbe_column_int32_t *)rcol;
  bool is_null = col->data[r] == col->null_value;
  if (!is_null) {
    __int32_t ori_data = col->data[r], big_base = 1;
    uint dec = f->decimals();
    for (uint i = 0; i < dec; ++i, big_base *= 10)
      ;
    *res = (double)ori_data / big_base;
    return false;
  }
  return true;
  return 0;
}

bool convert_int64_field_to_double(Field *f, monetdbe_column *rcol, int64_t r,
                                   double *res) {
  monetdbe_column_int64_t *col = (monetdbe_column_int64_t *)rcol;
  bool is_null = col->data[r] == col->null_value;
  if (!is_null) {
    __int64_t ori_data = col->data[r], big_base = 1;
    uint dec = f->decimals();
    for (uint i = 0; i < dec; ++i, big_base *= 10)
      ;
    *res = (double)ori_data / big_base;
    return false;
  }
  return true;
}

bool convert_int128_field_to_double(Field *f, monetdbe_column *rcol, int64_t r,
                                    double *res) {
  monetdbe_column_int128_t *col = (monetdbe_column_int128_t *)rcol;
  bool is_null = col->data[r] == col->null_value;
  if (!is_null) {
    __int128_t ori_data = col->data[r], big_base = 1;
    uint dec = f->decimals();
    for (uint i = 0; i < dec; ++i, big_base *= 10)
      ;
    *res = (double)ori_data / big_base;
    return false;
  }
  return true;
}

template <>
char *monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_NEWDECIMAL>(
    Field *f, monetdbe_column *rcol, int64_t r) {
  double decimal_value;
  bool is_null;
  switch (rcol->type) {
    case monetdbe_int8_t: {
      is_null = convert_int8_field_to_double(f, rcol, r, &decimal_value);
    } break;
    case monetdbe_int16_t: {
      is_null = convert_int16_field_to_double(f, rcol, r, &decimal_value);
    } break;
    case monetdbe_int32_t: {
      is_null = convert_int32_field_to_double(f, rcol, r, &decimal_value);
    } break;
    case monetdbe_int64_t: {
      is_null = convert_int64_field_to_double(f, rcol, r, &decimal_value);
    } break;
    case monetdbe_int128_t: {
      is_null = convert_int128_field_to_double(f, rcol, r, &decimal_value);
    } break;
    default: {
      DBUG_PRINT("mdb", ("CANNOT convert type: %d to NEWDECIMAL", f->type()));
      return "mdb type not new decimal";
    }
  }
  if (!is_null) {
    if (type_conversion_status::TYPE_OK != f->store(decimal_value)) {
      return "file store fail for int128_t";
    }
  }
  is_null ? f->set_null() : f->set_notnull();
  return NULL;
}

template <>
char *monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_SHORT>(
    Field *f, monetdbe_column *rcol, int64_t r) {
  if (rcol->type != monetdbe_int16_t) {
    DBUG_PRINT("mdb", ("ERROR: mdb col type:%d not int16_t", rcol->type));
    return "mdb type not int8_t";
  }

  monetdbe_column_int16_t *col = (monetdbe_column_int16_t *)rcol;
  bool is_null = col->data[r] == col->null_value;
  if (!is_null) {
    if (type_conversion_status::TYPE_OK !=
        f->store(col->data[r], f->is_unsigned())) {
      return "file store fail for int8_t";
    }
  }
  is_null ? f->set_null() : f->set_notnull();
  return NULL;
}

#endif /* MONETDB_RESULT_CONVERT_FIELD */
