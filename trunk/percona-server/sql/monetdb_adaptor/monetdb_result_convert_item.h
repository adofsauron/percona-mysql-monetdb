#ifndef MONETDB_RESULT_CONVERT_ITEM
#define MONETDB_RESULT_CONVERT_ITEM

#include "my_compiler.h"
#include "my_dbug.h"
#include "mysql/components/services/dynamic_privilege.h"
#include "mysql/components/services/udf_registration.h"
#include "mysql/plugin.h"
#include "mysql/service_plugin_registry.h"
#include "sql/mysqld.h"
#include "sql/sql_class.h"
#include "sql/query_result.h"
#include "sql/field.h"
#include "sql/item.h"
#include "sql/item_sum.h"
#include "sql/item_func.h"
#include "sql/visible_fields.h"
#include "mem_root_deque.h"
#include "monetdbe.h"
#include "sql/monetdb_adaptor/monetdb_result_convert_field.h"
#include "sql/monetdb_adaptor/monetdb_result_convert_func_sum.h"
#include "sql/monetdb_adaptor/monetdb_result_convert_func.h"

#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wreturn-type"

template <Item::Type>
char* monetdb_result_convert_item(Item* item, monetdbe_column *rcol, int64_t r) {
    return NULL;
}

template <>
char* monetdb_result_convert_item<Item::Type::FIELD_ITEM>(Item* item, monetdbe_column *rcol, int64_t r) {
    Item_field *ifield;
    Field *f;
	Item_ref *iref;

    if (Item::REF_ITEM == item->type()) {
        iref = (Item_ref *)item;
        ifield = (Item_field *)(iref->ref_item());
        f = ifield->result_field;
    } else {
        ifield = (Item_field *)item;
        f = ifield->result_field;
    }

    switch (f->type()) {
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_STRING: {
            return monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_STRING>(f, rcol, r);
        } break;
        case MYSQL_TYPE_LONG: 
        case MYSQL_TYPE_INT24: {
            return monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_INT24>(f, rcol, r);
        } break;
        case MYSQL_TYPE_LONGLONG: {
            return monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_LONGLONG>(f, rcol, r);
        } break;
        case MYSQL_TYPE_DATE: {
            return monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_DATE>(f, rcol, r);
        } break;
        case MYSQL_TYPE_TIME: {
            return monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_TIME>(f, rcol, r);
        } break;
        case MYSQL_TYPE_TIMESTAMP: {
            return monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_TIMESTAMP>(f, rcol, r);
        } break;
        case MYSQL_TYPE_TINY: {
            return monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_TINY>(f, rcol, r);
        } break;
        case MYSQL_TYPE_JSON: {
            return monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_JSON>(f, rcol, r);
        } break;
        case MYSQL_TYPE_YEAR: {
            return "date type year is not supported";
        } break;
        case MYSQL_TYPE_ENUM: {
            return "data type enum is not supported";
        } break;
        case MYSQL_TYPE_SET: {
            return "data type set is not supported";
        } break;
        case MYSQL_TYPE_TINY_BLOB: {
            return "data type tiny blob is not supported";
        } break;
        case MYSQL_TYPE_MEDIUM_BLOB: {
            return "data type medium blob is not supported";
        } break;
        case MYSQL_TYPE_LONG_BLOB: {
            return "data type long blob is not supported";
        } break;
        case MYSQL_TYPE_BLOB: {
            return monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_BLOB>(f, rcol, r);
        } break;
        case MYSQL_TYPE_GEOMETRY: {
            return "data type GEOMETRY is not supported";
        } break;
        case MYSQL_TYPE_FLOAT: {
            return monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_FLOAT>(f, rcol, r);
        } break;
        case MYSQL_TYPE_DOUBLE: {
            return monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_DOUBLE>(f, rcol, r);
        } break;
        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL: {
            return monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_NEWDECIMAL>(f, rcol, r);
        } break;
        case MYSQL_TYPE_SHORT: {
            return monetdb_result_convert_field<enum_field_types::MYSQL_TYPE_SHORT>(f, rcol, r);
        } break;
        case MYSQL_TYPE_NULL:
            break;
        default: {
            DBUG_PRINT("mdb", ("UNKNOWN field type: %d", f->type()));
        }
    }
                
    return NULL;
}

template <>
char* monetdb_result_convert_item<Item::Type::SUM_FUNC_ITEM>(Item* item, monetdbe_column *rcol, int64_t r) {
    Item_sum *item_sum = dynamic_cast<Item_sum *>(item);

    switch (item_sum->sum_func()) {
        case Item_sum::Sumfunctype::COUNT_DISTINCT_FUNC:
        case Item_sum::Sumfunctype::COUNT_FUNC: {
            return monetdb_result_convert_func_sum<Item_sum::Sumfunctype::COUNT_FUNC>(item_sum, rcol, r);
        } break;
        case Item_sum::Sumfunctype::SUM_DISTINCT_FUNC:
        case Item_sum::Sumfunctype::SUM_FUNC:{
            return monetdb_result_convert_func_sum<Item_sum::Sumfunctype::SUM_FUNC>(item_sum, rcol, r);
        } break;
        case Item_sum::Sumfunctype::AVG_DISTINCT_FUNC:
        case Item_sum::Sumfunctype::AVG_FUNC: {
            return monetdb_result_convert_func_sum<Item_sum::Sumfunctype::AVG_FUNC>(item_sum, rcol, r);
        case Item_sum::Sumfunctype::MIN_FUNC:
        case Item_sum::Sumfunctype::MAX_FUNC:
            return monetdb_result_convert_func_sum<Item_sum::Sumfunctype::MAX_FUNC>(item_sum, rcol, r);
        case Item_sum::Sumfunctype::STD_FUNC:
            return "no such unary operator 'STD_FUNC'";
        case Item_sum::Sumfunctype::VARIANCE_FUNC:
            return "no such unary operator 'VARIANCE_FUNC'";
        case Item_sum::Sumfunctype::UDF_SUM_FUNC:
            return "no such unary operator 'UDF_SUM_FUNC'";
        case Item_sum::Sumfunctype::GROUP_CONCAT_FUNC:
            return monetdb_result_convert_func_sum<Item_sum::Sumfunctype::GROUP_CONCAT_FUNC>(item_sum, rcol, r);
        } break;
        default:
            DBUG_PRINT("mdb", ("UNKNOWN item sum type: %d",
                        item_sum->sum_func()));
    }

    return NULL;
}

template <>
char* monetdb_result_convert_item<Item::Type::FIELD_AVG_ITEM>(Item* item, monetdbe_column *rcol, int64_t r) {
    if (rcol->type != monetdbe_int64_t) {
        return "col type not match int64_t when item field avg";
    }

    monetdbe_column_int64_t *col = (monetdbe_column_int64_t *) rcol;
    if (col->data[r] != col->null_value) {
        Item_avg_field *item_avg_field = dynamic_cast<Item_avg_field *>(item);
        Field *f = item_avg_field->field;
        bool is_null = col->data[r] == col->null_value;
        if (!is_null) {
            type_conversion_status status = f->store((double) col->data[r]);
                if (type_conversion_status::TYPE_OK != status) {
                DBUG_PRINT("mdb",
                            ("ERROR: field store fail for avg item, status: %d", status));
                return "field store fail for avg item";
            }
            memcpy(f->field_ptr() + sizeof(double), "1", 1);
            item_avg_field->hybrid_type = REAL_RESULT;
            item_avg_field->fixed = true;
            item_avg_field->null_value = false;
        }
        is_null ? f->set_null() : f->set_notnull();
    }

    return NULL;
}

template <>
char* monetdb_result_convert_item<Item::Type::FUNC_ITEM>(Item* item, monetdbe_column *rcol, int64_t r) {
    Item_func *item_func = dynamic_cast<Item_func *>(item);

    switch (item_func->functype()) {
        case Item_func::Functype::DIV_FUNC: {
            return monetdb_result_convert_func<Item_func::Functype::DIV_FUNC>(item_func, rcol, r);
        } break;
        default:
            DBUG_PRINT("mdb", ("UNKNOWN item func type: %d",
                        item_func->functype()));
    }

    return NULL;
}

#endif /* MONETDB_RESULT_CONVERT_ITEM */
