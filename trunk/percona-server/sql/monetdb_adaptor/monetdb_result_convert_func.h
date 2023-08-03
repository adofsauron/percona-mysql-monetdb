#ifndef MONETDB_RESULT_CONVERT_FUNC
#define MONETDB_RESULT_CONVERT_FUNC

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
#include "sql/item_func.h"
#include "sql/visible_fields.h"
#include "mem_root_deque.h"
#include "monetdbe.h"

#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wreturn-type"

template <Item_func::Functype>
char* monetdb_result_convert_func(Item_func* item_func, monetdbe_column *rcol, int64_t r) {
    return NULL;
}

template <>
char* monetdb_result_convert_func<Item_func::Functype::DIV_FUNC>(Item_func* item_func, monetdbe_column *rcol, int64_t r) {
    if (rcol->type != monetdbe_int128_t) {
        return "col type not match int128_t when item field div func";
    }

    monetdbe_column_int128_t *col = (monetdbe_column_int128_t *)rcol;
    if (col->data[r] != col->null_value) {
        Item_func_div *item_func_div = dynamic_cast<Item_func_div *>(item_func);
        unsigned int scale = rcol[0].sql_type.scale;
        double data_double = (double)  col->data[r];
        while (scale--) {
            data_double /= 10;
        }
        if (double2my_decimal(E_DEC_FATAL_ERROR, data_double, &item_func_div->m_decimal)) {
            DBUG_PRINT("mdb", ("ERROR: col div func convert double to decimal fail"));
            return "col div func convert double to decimal fail";
        }

        item_func_div->hybrid_type = DECIMAL_RESULT;
        item_func_div->fixed = true;
        item_func_div->m_force_fixed = true;
    }

    return NULL;
}

#endif /* MONETDB_RESULT_CONVERT_FUNC */
