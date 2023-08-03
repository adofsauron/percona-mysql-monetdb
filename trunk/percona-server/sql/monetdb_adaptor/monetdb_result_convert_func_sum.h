#ifndef MONETDB_RESULT_CONVERT_FUNC_SUM
#define MONETDB_RESULT_CONVERT_FUNC_SUM

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
#include "sql/visible_fields.h"
#include "mem_root_deque.h"
#include "monetdbe.h"

#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wreturn-type"

template <Item_sum::Sumfunctype>
char* monetdb_result_convert_func_sum(Item_sum *item_sum, monetdbe_column *rcol, int64_t r) {
    return NULL;
}

template <>
char* monetdb_result_convert_func_sum<Item_sum::Sumfunctype::COUNT_FUNC>(Item_sum *item_sum, monetdbe_column *rcol, int64_t r) {
    if (rcol->type != monetdbe_int64_t) {
        return "col type not match int64 when item func count";
    }

    monetdbe_column_int64_t *col = (monetdbe_column_int64_t *)rcol;
    if (col->data[r] != col->null_value) {
        Item_sum_count *item_sum_count = dynamic_cast<Item_sum_count *>(item_sum);
        item_sum_count->make_const(col->data[r]);
    }
    return NULL;
}

template <>
char* monetdb_result_convert_func_sum<Item_sum::Sumfunctype::SUM_FUNC>(Item_sum *item_sum, monetdbe_column *rcol, int64_t r) {
    if (rcol->type != monetdbe_int128_t) {
        return "col type not match int128 when item func sum";
    }

    monetdbe_column_int128_t *col = (monetdbe_column_int128_t *)rcol;
    if (col->data[r] != col->null_value) {
        Item_sum_sum *item_sum_sum = dynamic_cast<Item_sum_sum *>(item_sum);
        item_sum_sum->hybrid_type = DECIMAL_RESULT;
        item_sum_sum->curr_dec_buff = 0;
        my_decimal* decimal_value = &item_sum_sum->dec_buffs[0];
        unsigned int scale = rcol[0].sql_type.scale;
        double data_double = (double)  col->data[r];
        while (scale--) {
            data_double /= 10;
        }
        if (double2my_decimal(E_DEC_FATAL_ERROR,  data_double, decimal_value)) {
            DBUG_PRINT("mdb", ("ERROR: col sum convert double to decimal fail"));
            return "col sum convert double to decimal fail";
        }

        item_sum_sum->null_value = false;
    }
    return NULL;
}

template <>
char* monetdb_result_convert_func_sum<Item_sum::Sumfunctype::AVG_FUNC>(Item_sum *item_sum, monetdbe_column *rcol, int64_t r) {
    if (rcol->type != monetdbe_double) {
        return "col type not match double when item func avg";
    }

    monetdbe_column_double *col = (monetdbe_column_double *)rcol;
    if (col->data[r] != col->null_value) {
        Item_sum_avg *item_sum_avg = dynamic_cast<Item_sum_avg *>(item_sum);
        item_sum_avg->hybrid_type = DECIMAL_RESULT;
        item_sum_avg->curr_dec_buff = 0;
        my_decimal* decimal_value = &item_sum_avg->dec_buffs[0];
        unsigned int scale = rcol[0].sql_type.scale;
        double data_double = (double) col->data[r];
        while (scale--) {
            data_double /= 10;
        }
        if (double2my_decimal(E_DEC_FATAL_ERROR, data_double, decimal_value)) {
            DBUG_PRINT("mdb", ("ERROR: col avg convert double to decimal fail"));
            return "col avg convert double to decimal fail";
        }
        item_sum_avg->m_count = 1;
        item_sum_avg->null_value = false;
    }

    return NULL;
}

template <>
char* monetdb_result_convert_func_sum<Item_sum::Sumfunctype::MAX_FUNC>(Item_sum *item_sum, monetdbe_column *rcol, int64_t r) {
    if (rcol->type != monetdbe_int32_t) {
        return "col type not match double when item func min/max";
    }

    monetdbe_column_int32_t *col = (monetdbe_column_int32_t *)rcol;
    if (col->data[r] != col->null_value) {
        Item_sum_hybrid *item_sum_hybrid = dynamic_cast<Item_sum_hybrid *>(item_sum);

        bool set_value_over = false;
        Item_cache_int *item_cache_int = dynamic_cast<Item_cache_int *>(item_sum_hybrid->value);
        if (item_cache_int) {
            item_cache_int->value = (longlong) col->data[r];
            item_cache_int->fixed = 1;
            item_cache_int->null_value = false;
            item_cache_int->value_cached = true;
            set_value_over = true;
        }

        if (!set_value_over) {
            Item_cache_decimal *item_cache_decimal = dynamic_cast<Item_cache_decimal *>(item_sum_hybrid->value);
            if (item_cache_decimal) {
                unsigned int scale = rcol[0].sql_type.scale;
                double data_double = (double) col->data[r];
                while (scale--) {
                    data_double /= 10;
                }
                if (double2my_decimal(E_DEC_FATAL_ERROR, data_double, &item_cache_decimal->decimal_value)) {
                    DBUG_PRINT("mdb", ("ERROR: col sum func convert longlong to decimal fail"));
                    return "col sum func convert longlong to decimal fail";
                }
                item_cache_decimal->fixed = 1;
                item_cache_decimal->null_value = false;
                item_cache_decimal->value_cached = true;
                set_value_over = true;
            }
        }

        if (!set_value_over) {
            DBUG_PRINT("mdb", ("ERROR: col sum func not Item_cache_int, Item_cache_decimal"));
            return "col sum func not Item_cache_int, Item_cache_decimal";
        }

        item_sum_hybrid->null_value = false;
        item_sum_hybrid->fixed = 1;
    }

    return NULL;
}

template <>
char* monetdb_result_convert_func_sum<Item_sum::Sumfunctype::GROUP_CONCAT_FUNC>(Item_sum *item_sum, monetdbe_column *rcol, int64_t r) {
    if (rcol->type != monetdbe_str) {
        return "col type not match double when item func group concat";
    }

    monetdbe_column_str *col = (monetdbe_column_str *)rcol;
    if (col->is_null(col->data + r) == 0) {
        Item_func_group_concat *item_func_group_concat = dynamic_cast<Item_func_group_concat *>(item_sum);
        String *str_value = &item_func_group_concat->result;
        str_value->set(col->data[r], strlen(col->data[r]), str_value->charset());

        item_func_group_concat->null_value = false;
        item_func_group_concat->fixed = 1;
        item_func_group_concat->m_result_finalized = true;
    }

    return NULL;
}

#endif /* MONETDB_RESULT_CONVERT_FUNC_SUM */
