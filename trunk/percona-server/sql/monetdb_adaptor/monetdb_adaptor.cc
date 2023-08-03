#include "monetdb_adaptor.h"
#include "sql/monetdb_adaptor/monetdb_result_convert_item.h"
#include "sql/current_thd.h"
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <regex>
#include <vector>

#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wswitch"

Mondetdb_Adaptor *Mondetdb_Adaptor::monetdb_adaptor = nullptr;

Mondetdb_Adaptor *Mondetdb_Adaptor::instance(){
  if (unlikely(!monetdb_adaptor)) {
    monetdb_adaptor = new Mondetdb_Adaptor();
  }

  return monetdb_adaptor;
}

void Mondetdb_Adaptor::init_dbfarm_dir() {
  if ((access(m_dbfarm_dir.c_str(), F_OK) == 0)) {
		DBUG_PRINT("mdb", ("mdb dbfarm_dir: %s already exists", m_dbfarm_dir.c_str()));
    return;
	}

#ifdef USE_MONETDBD_API
	const std::string monetdbd_path = m_bin_dir + "monetdbd";
  if ((access(monetdbd_path.c_str(), X_OK) != 0)) {
		DBUG_PRINT("mdb", ("monetdbd: %s not exists or execute permission", monetdbd_path.c_str()));
    return;
  }

	const std::string monetdbd_create = monetdbd_path + " create " + m_dbfarm_dir;
	assert(0 == system(monetdbd_create.c_str()));
#else
  const std::string command = std::string("mkdir -p ") + m_dbfarm_dir.c_str();
	assert(0 == system(command.c_str()));
#endif

	DBUG_PRINT("mdb", ("mdb init dbfarm: %s over", m_dbfarm_dir.c_str()));
}

void Mondetdb_Adaptor::init() {
  m_dbfarm_dir = std::string(mysql_real_data_home) + m_dbfarm_dir_name;
	m_bin_dir = std::string(mysql_home_ptr) + m_bin_name;

	m_db_options = {.memorylimit = -1,
                        .querytimeout = -1,
                        .sessiontimeout = -1,
                        .nr_threads = -1,
                        .remote = NULL,
                        .mapi_server = NULL,
                        .trace_file = NULL};

	init_dbfarm_dir();

	m_status = Monetdb_Status::MS_INITED;
	DBUG_PRINT("mdb", ("mdb m_dbfarm_dir: %s", m_dbfarm_dir.c_str()));
  DBUG_PRINT("mdb", ("mdb m_bin_dir: %s", m_bin_dir.c_str()));
  DBUG_PRINT("mdb", ("mdb adaptor init over"));
}

void Mondetdb_Adaptor::destroy() {
  if (!m_curr_database) {
		DBUG_PRINT("mdb", ("mdb adaptor destroy over"));
    return;
  }

  assert(0 == monetdbe_close(*m_curr_database)); // only when destroy
	DBUG_PRINT("mdb", ("mdb close database: %s over", m_curr_dbname.c_str()));
  
	delete m_curr_database;
  m_curr_database = nullptr;
	m_curr_dbname.clear();

	m_status = Monetdb_Status::MS_NONE;
	DBUG_PRINT("mdb", ("mdb adaptor destroy over"));
}

monetdbe_database *Mondetdb_Adaptor::get_database(const char *dbname) {
  if (unlikely(!dbname)) {
    return nullptr;
  }

	if (m_database_container.empty()) {
    return nullptr;
	}

	auto database = m_database_container.find(dbname);
	if (m_database_container.end() == database) {
    return nullptr;
	}

	return database->second;
}

#ifdef USE_MONETDBD_API
bool Mondetdb_Adaptor::create_monetdb_database(const char *dbname) {
	const std::string full_dbname = m_dbfarm_dir + dbname;
	if ((access(full_dbname.c_str(), F_OK) == 0)) {
    DBUG_PRINT("mdb",
               ("mdb full_dbname: %s already exists", full_dbname.c_str()));
    return true;
  }

  const std::string monetdb_path = m_bin_dir + "mdb";
  if ((access(monetdb_path.c_str(), X_OK) != 0)) {
    DBUG_PRINT("mdb", ("ERROR: mdb: %s not exists or execute permission",
                           monetdb_path.c_str()));
    return false;
  }

	const std::string monetdb_create =
			monetdb_path + " create " + dbname;
	assert(0 == system(monetdb_create.c_str()));

	const std::string monetdb_release =
            monetdb_path + " release " + dbname;
  assert(0 == system(monetdb_release.c_str()));

	DBUG_PRINT("mdb", ("create mdb database: %s over", dbname));
  return true;
}
#endif

char* Mondetdb_Adaptor::close_curr_database() {
	if (!m_curr_database) {
    return NULL;
	}

	if (0 != monetdbe_close(*m_curr_database)) {
    DBUG_PRINT("mdb",
               ("ERROR: mdb monetdbe_close: %s fail, msg: %s",
                m_curr_dbname.c_str(), monetdbe_error(*m_curr_database)));
    return "close curr db fail";
  }
	
	delete m_curr_database;
  m_curr_database = nullptr;
	m_curr_dbname.clear();

	return NULL;
}

monetdbe_database *Mondetdb_Adaptor::open_database(const char *dbname) {
  if (unlikely(!dbname)) {
    return nullptr;
  }

	if (!m_curr_dbname.empty()) {
    if ((m_curr_dbname == dbname) && m_curr_database) {
      return m_curr_database;
    }

		if (close_curr_database() != NULL) {
      DBUG_PRINT("mdb",
                 ("ERROR: mdb monetdclose_curr_databasebe_close fail"));
      return nullptr;
    }
	}

#ifdef USE_MONETDBD_API
	if (!create_monetdb_database(dbname)) {
		DBUG_PRINT("mdb", ("ERROR: mdb create_monetdb_database dbname: %s fail", dbname));
    return nullptr;
	}
#endif

	const std::string full_dbname = m_dbfarm_dir + dbname;
	monetdbe_database *new_db = new monetdbe_database;
  int err = 0;
	if ((err = monetdbe_open(new_db, (char*) (full_dbname.c_str()), NULL)) != 0) {
		DBUG_PRINT("mdb", ("ERROR: mdb open monetdbe: %s fail, err: %d, msg: %s", 
			full_dbname.c_str(), err, monetdbe_error(*new_db)));

    delete new_db;
    new_db = nullptr;
    return nullptr;
	}

	m_curr_dbname = dbname;
	m_curr_database = new_db;

	DBUG_PRINT("mdb", ("mdb open_database: %s ok", full_dbname.c_str()));
	return new_db;
}

char *Mondetdb_Adaptor::drop_database(const char *dbname) {
#ifndef MONETDB_USE
  return NULL;
#endif

  if (unlikely(!dbname)) {
    return "no dbname";
  }

	std::lock_guard<std::mutex> lock(m_mutex);
	m_status = Monetdb_Status::MS_IN_PROCESS;
	if ((m_curr_dbname == dbname) && (m_curr_database)) {
    if (0 != monetdbe_close(*m_curr_database)) {
      DBUG_PRINT("mdb", ("ERROR: mdb close database: %s fail", dbname));
			m_status = Monetdb_Status::MS_WAIT;
      return "close db fail";
    }

    delete m_curr_database;
    m_curr_database = nullptr;
    m_curr_dbname.clear();
	}

	const std::string full_dbname = m_dbfarm_dir + dbname;
  if ((access(full_dbname.c_str(), F_OK) == 0)) {
    const std::string command = std::string("rm -rf ") + full_dbname.c_str();
    if (0 != system(command.c_str())) {
			DBUG_PRINT("mdb", ("ERROR: mdb drop database: %s fail, rm path: %s fail", dbname, full_dbname.c_str()));
			m_status = Monetdb_Status::MS_WAIT;
			return "rm db path fail";
		}
  }

	m_status = Monetdb_Status::MS_WAIT;
	DBUG_PRINT("mdb", ("mdb drop database: %s path: %s", dbname, full_dbname.c_str()));
	return NULL;
}

char *Mondetdb_Adaptor::execute_query(monetdbe_database *db, char *query,
                                      monetdbe_result **result,
                                      monetdbe_cnt *affected_rows) {
	if (unlikely(!db)) {
    return "database not open";
	}

  if (unlikely(!query)) {
    return "no query";
  }

	std::lock_guard<std::mutex> lock(m_mutex);
	m_status = Monetdb_Status::MS_IN_PROCESS;
	char* err = NULL;
  if ((err = monetdbe_query(*db, query, result, affected_rows)) != NULL) {
		DBUG_PRINT("mdb", ("ERROR: mdb execute_query: %s err: %s", query, err));
		m_status = Monetdb_Status::MS_WAIT;
    return err;
  }

	m_status = Monetdb_Status::MS_WAIT;
	DBUG_PRINT("mdb", ("mdb execute_query: %s", query));
	return NULL;
}

char *Mondetdb_Adaptor::execute_query(const char *dbname, std::string& query_str, bool set_status) {
#ifndef MONETDB_USE
  return NULL;
#endif

  if (unlikely(!dbname)) {
    return "no dbname";
  }

  if (unlikely(query_str.empty())) {
    return "no query";
  }

	std::lock_guard<std::mutex> lock(m_mutex);
	m_status = Monetdb_Status::MS_IN_PROCESS;
	monetdbe_database *db = NULL;
  if ((db = open_database(dbname)) == NULL) {
    DBUG_PRINT("mdb", ("ERROR: mdb open_database fail, dbname: %s", dbname));
		m_status = Monetdb_Status::MS_WAIT;
    return "open_database fail";
  }

  query_legal_covert(query_str);
  const char *query = query_str.c_str();

  monetdbe_cnt affected_rows;
	char* err = NULL;
	if ((err = monetdbe_query(*db, (char *) query, NULL, &affected_rows)) != NULL) {
    DBUG_PRINT("mdb", 
			("ERROR: mdb monetdbe_query fail, dbname: %s query: %s err: %s", dbname, query, err));
		m_status = Monetdb_Status::MS_WAIT;
    return err;
	}

  if (set_status) {
    assert(current_thd);
    Diagnostics_area *da = current_thd->get_stmt_da();
    assert(da);
    if (!da->is_set()) {
      da->set_ok_status(affected_rows,0,NULL);
    }
  }
  
	m_status = Monetdb_Status::MS_WAIT;
  DBUG_PRINT("mdb", ("mdb exec ok, query: %s", query));
	return NULL;
}

char *Mondetdb_Adaptor::execute_query(const char *dbname, std::string &query_str,
                                      THD *thd, mem_root_deque<Item *> &values,
                                      Query_result &query_result, 
                                      ha_rows &send_records_ptr) {
#ifndef MONETDB_USE
  return NULL;
#endif

  if (unlikely(!dbname)) {
    return "no dbname";
  }

  if (unlikely(query_str.empty())) {
    return "no query";
  }

	std::lock_guard<std::mutex> lock(m_mutex);
	m_status = Monetdb_Status::MS_IN_PROCESS;
  monetdbe_database *db = NULL;
  if ((db = open_database(dbname)) == NULL) {
    DBUG_PRINT("mdb",
               ("ERROR: mdb open_database fail, dbname: %s", dbname));
    return "open_database fail";
  }

  query_legal_covert(query_str);
  const char *query = query_str.c_str();

  monetdbe_result *result = NULL;
  char *err = NULL;
  if ((err = monetdbe_query(*db, (char *) query_str.c_str(), &result, NULL)) != NULL) {
    DBUG_PRINT(
        "mdb",
        ("ERROR: mdb monetdbe_query fail, dbname: %s query: %s err: %s",
         dbname, query, err));
		m_status = Monetdb_Status::MS_WAIT;
    return err;
  }

	m_status = Monetdb_Status::MS_WAIT;
  // assert(result->ncols == values.size());
  DBUG_PRINT(
      "mdb",
      ("query: %s result with %zu cols and %" PRId64 " rows, values.size: %zu",
       query, result->ncols, result->nrows, values.size()));

  bool convert_ok = true;
  for (int64_t r = 0; r < result->nrows; r++) {
    size_t c = 0;
    for (Item *item : VisibleFields(values)) {
      monetdbe_column *rcol;
      if ((err = monetdbe_result_fetch(result, &rcol, c++)) != NULL) {
				DBUG_PRINT("mdb", ("ERROR: mdb monetdbe_result_fetch fail, err: %s", err));
        return err;
      }

      convert_ok = true;
      switch (item->type()) {
        case Item::Type::SUM_FUNC_ITEM: {
          if ((err = monetdb_result_convert_item<Item::Type::SUM_FUNC_ITEM>(item, rcol, r)) != NULL) {
            convert_ok = false;
            goto CONVERT_ERR;
          }
        } break;
        case Item::Type::DEFAULT_VALUE_ITEM:
        case Item::Type::REF_ITEM:
        case Item::Type::FIELD_ITEM: { 
          if ((err = monetdb_result_convert_item<Item::Type::FIELD_ITEM>(item, rcol, r)) != NULL) {
            convert_ok = false;
            goto CONVERT_ERR;
          }
        }  break;
        case Item::Type::FIELD_AVG_ITEM: {
          if ((err = monetdb_result_convert_item<Item::Type::FIELD_AVG_ITEM>(item, rcol, r)) != NULL) {
            convert_ok = false;
            goto CONVERT_ERR;
          }
        } break;
        case Item::Type::FUNC_ITEM: {
          if ((err = monetdb_result_convert_item<Item::Type::FUNC_ITEM>(item, rcol, r)) != NULL) {
            convert_ok = false;
            goto CONVERT_ERR;
          }
        } break;
        default:
          DBUG_PRINT("mdb", ("UNKNOWN item type: %d", item->type()));
      }
		}

    ++send_records_ptr;
		if (query_result.send_data(thd, values)) {
			DBUG_PRINT("mdb", ("ERROR: mdb send_data fail"));
      return "query_result send_data fail";
    }

    thd->get_stmt_da()->inc_current_row_for_condition();
	}

CONVERT_ERR:
  if ((err = monetdbe_cleanup_result(*db, result)) != NULL) {
		DBUG_PRINT("mdb", ("ERROR: mdb monetdbe_cleanup_result fail, err: %s", err));
    return err;
  }

  if (!convert_ok) {
    DBUG_PRINT("mdb", ("ERROR: mdb convert retult fail, err: %s", err));
    return err;
  }

	DBUG_PRINT("mdb", ("mdb exec: %s, query: %s ", convert_ok ? "ok" : "false", query));
	return NULL;
}

bool Mondetdb_Adaptor::dbname_legal(const char *dbname) {
  if (unlikely(!dbname)) {
    return false;
  }

  return m_mysql_system_dbnames.end() == m_mysql_system_dbnames.find(dbname);
}

bool Mondetdb_Adaptor::query_legal(const char *query) {
  if (unlikely(!query)) {
    return false;
  }

  return m_mysql_system_querys.end() == m_mysql_system_querys.find(query);
}

void Mondetdb_Adaptor::query_legal_covert(std::string &sql) {
  if (unlikely(sql.empty())) {
    return;
  }

  // '`' to '"'
  std::replace(sql.begin(), sql.end(), '`', '"');
}

char *Mondetdb_Adaptor::execute_load(const char *dbname, const char *load_string) {
#ifndef MONETDB_USE
  return NULL;
#endif

  if (unlikely(!dbname)) {
    return "no dbname";
  }

  if (unlikely(!load_string)) {
    return "no load_string";
  }

  std::string origin_load_string(load_string);

  std::string s;
  s.append(1, ' ');
  std::regex reg(s);
  std::vector<std::string> load_string_elems(std::sregex_token_iterator(origin_load_string.begin(), origin_load_string.end(), reg, -1),
                                  std::sregex_token_iterator());

  if (load_string_elems.empty()) {
    DBUG_PRINT("mdb", ("ERROR: execute_load load_string_elems empty, load_string: %s", load_string));
    return "no load_string_elems by ' '";
  }

  std::string table_name;
  std::string terminate_char;
  std::string file_path;

  for (auto it = load_string_elems.begin(); it != load_string_elems.end(); ++it) {
    if (it == (load_string_elems.end()-1)) {
      break;
    }

    const std::string &elem = *it;
    // table name
    if (("table" == elem) || ("TABLE" == elem)) {
      table_name = *(it+1);
    }
    // terminated, TODO: hack solution
    if ((("terminated" == elem) || ("TERMINATED" == elem)) && ((it != (load_string_elems.end()-2)))) {
      terminate_char = *(it+2);
    }
    // file path
    if (("infile" == elem) || ("INFILE" == elem)) {
      file_path = *(it+1);
    }
  }

  if (table_name.empty()) {
    DBUG_PRINT("mdb", ("ERROR: execute_load no table_name"));
    return "no table_name";
  }

  if (terminate_char.empty()) {
    DBUG_PRINT("mdb", ("ERROR: execute_load no terminate_char"));
    return "no terminate_char";
  }

  if (file_path.empty()) {
    DBUG_PRINT("mdb", ("ERROR: execute_load no file_path"));
    return "no file_path";
  }

  std::string mdb_copy_sring("COPY INTO ");
  mdb_copy_sring.append(table_name);
  mdb_copy_sring.append(" FROM ");
  mdb_copy_sring.append(file_path);
  mdb_copy_sring.append(" USING DELIMITERS ");
  mdb_copy_sring.append(terminate_char);
  mdb_copy_sring.append(", '\n';"); 

  DBUG_PRINT("mdb", ("INFO: execute_load { %s }", mdb_copy_sring.c_str()));

  return execute_query(dbname, mdb_copy_sring, true);
}