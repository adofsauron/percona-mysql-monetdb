#ifndef MONETDB_ADAPTOR_H
#define MONETDB_ADAPTOR_H

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

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>
#include <mutex>

class Mondetdb_Adaptor {
 private:
  Mondetdb_Adaptor() = default;
  ~Mondetdb_Adaptor() = default;

 public:
  enum class Monetdb_Status {
    MS_NONE,
    MS_INITED,
    MS_IN_PROCESS,
    MS_WAIT,
    MS_END
  };

public:
  static Mondetdb_Adaptor *instance();
  void init();
  void destroy();

public:
  bool dbname_legal(const char *dbname);
  bool query_legal(const char *query);

public:
  char *drop_database(const char *dbname);
  char *execute_query(const char *dbname, std::string &query_str, bool set_status = false);
  char *execute_query(const char *dbname, std::string &query_str,
                      THD *thd, mem_root_deque<Item *> &values, Query_result &query_result, 
                      ha_rows &send_records_ptr);

public:
  char *execute_load(const char *dbname, const char *load_string);

private:
  monetdbe_database *get_database(const char *dbname);
  monetdbe_database *open_database(const char *dbname);
  char *execute_query(monetdbe_database *db, char *query,
                      monetdbe_result **result, monetdbe_cnt *affected_rows);

private:
  void init_dbfarm_dir();
	char *close_curr_database();
  void query_legal_covert(std::string &sql);

#ifdef USE_MONETDBD_API
	bool create_monetdb_database(const char *dbname);
#endif

private:
  static Mondetdb_Adaptor *monetdb_adaptor;
	static constexpr const char *m_dbfarm_dir_name = ".#__mdb_dbfarm__/";
	static constexpr const char *m_bin_name = "bin/";
  
private:
	std::unordered_map<std::string, monetdbe_database *> m_database_container;
	std::string m_dbfarm_dir;
	std::string m_bin_dir;
	monetdbe_options m_db_options;
  std::string m_curr_dbname;
  monetdbe_database *m_curr_database = nullptr; 
	Monetdb_Status m_status = Monetdb_Status::MS_NONE;
	std::mutex m_mutex;

private:
  const std::unordered_set<std::string> m_mysql_system_dbnames {"sys", "mysql", 
    "information_schema", "performance_schema"};
  const std::unordered_set<std::string> m_mysql_system_querys {"select @@version_comment limit 1", 
    "SELECT DATABASE()",
    "SELECT VERSION()",
    "select version()"};

};

#endif // MONETDB_ADAPTOR_H
