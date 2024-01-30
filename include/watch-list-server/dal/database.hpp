/**
 * @file database.hpp
 * @author denis
 * @brief
 * @date 12024-01-20
 *
 */

#ifndef DATABASE_HPP_
#define DATABASE_HPP_

#include <rapidjson/document.h>
#include <uuid.h>
#include <uuid/uuid.h>

#include <functional>
#include <initializer_list>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "watch-list-server/json/json-utils.hpp"
#include "watch-list-server/server-generic-error.hpp"
#include "watch-list-server/settings/server-settings-models.hpp"

namespace watch_list_app::server::dal {

class Database {
 public:
  using TableName = std::string_view;
  using DataModel = rapidjson::Value;
  using DataModelConstRef = DataModel const&;

 private:
  static std::shared_ptr<Database> instance_;

  std::string database_path_;  // TODO - Replace with actual database.
  rapidjson::Document data_;   // TODO - Replace with actual database.

 private:
  explicit Database(std::string database_path) : database_path_(std::move(database_path)) {}

 public:
  static std::shared_ptr<Database> instance() {
    return instance_;
  }
  [[nodiscard]] static OptionalServerGenericError initialize(settings::ServerSettingsDatabase const& settings);
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Value> find(TableName table_name, std::string const& uuid);
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Document> scan(TableName table_name);
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Document> add(TableName table_name, DataModelConstRef data);
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Document> add(
      TableName table_name, std::initializer_list<std::reference_wrapper<rapidjson::Value const>> data);
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Document> replace(TableName table_name, DataModelConstRef data);
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Document> remove(TableName table_name, void* key);
};

}  // namespace watch_list_app::server::dal

#endif  // DATABASE_HPP_
