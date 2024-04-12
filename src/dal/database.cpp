/**
 * @file database.cpp
 * @author denis
 * @brief
 * @date 12024-01-20
 *
 */

#include "watch-list-server/dal/database.hpp"

#include <fmt/core.h>

#include <memory>

namespace watch_list_app::server::dal {
std::shared_ptr<Database> Database::instance_ = nullptr;

Database::Database(std::string const& database_name) : logger_(database_name) {}

OptionalServerGenericError Database::initialize() {
  return initialize_impl();
}

ServerGenericErrorVariant<rapidjson::Value> Database::find(TableName table_name, std::string const& uuid) {
  return find_impl(table_name, uuid);
}

ServerGenericErrorVariant<rapidjson::Document> Database::scan(TableName table_name) {
  return scan_impl(table_name);
}

ServerGenericErrorVariant<rapidjson::Document> Database::add(TableName table_name, DataModelConstRef data) {
  return add_impl(table_name, data);
}

ServerGenericErrorVariant<rapidjson::Document> Database::add(TableName table_name, std::initializer_list<DataModelConstRef> data) {
  return add_impl(table_name, data);
}

ServerGenericErrorVariant<rapidjson::Document> Database::replace(TableName table_name, DataModelConstRef data) {
  return replace_impl(table_name, data);
}

ServerGenericErrorVariant<rapidjson::Document> Database::remove(TableName table_name, void* key) {
  return remove_impl(table_name, key);
}

}  // namespace watch_list_app::server::dal
