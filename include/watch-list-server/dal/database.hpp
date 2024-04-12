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
#include "watch-list-server/server-logger.hpp"
#include "watch-list-server/settings/server-settings-models.hpp"

namespace watch_list_app::server::dal {

class Database {
 public:
  using TableName = std::string_view;
  using DataModel = rapidjson::Value;
  using DataModelConstRef = std::reference_wrapper<DataModel const>;

 private:
  static std::shared_ptr<Database> instance_;

  friend class DatabaseBuilder;

 protected:
  ServerLogger logger_;

 protected:
  explicit Database(std::string const& database_name);
  virtual ~Database() = default;

  [[nodiscard]] virtual OptionalServerGenericError initialize_impl() = 0;
  [[nodiscard]] virtual ServerGenericErrorVariant<rapidjson::Value> find_impl(TableName table_name, std::string const& uuid) = 0;
  [[nodiscard]] virtual ServerGenericErrorVariant<rapidjson::Document> scan_impl(TableName table_name) = 0;
  [[nodiscard]] virtual ServerGenericErrorVariant<rapidjson::Document> add_impl(TableName table_name, DataModelConstRef data) = 0;
  [[nodiscard]] virtual ServerGenericErrorVariant<rapidjson::Document> add_impl(TableName table_name,
                                                                                std::initializer_list<DataModelConstRef> data) = 0;
  [[nodiscard]] virtual ServerGenericErrorVariant<rapidjson::Document> replace_impl(TableName table_name, DataModelConstRef data) = 0;
  [[nodiscard]] virtual ServerGenericErrorVariant<rapidjson::Document> remove_impl(TableName table_name, void* key) = 0;

 public:
  static std::shared_ptr<Database> instance() {
    return instance_;
  }
  [[nodiscard]] OptionalServerGenericError initialize();
  /// @brief Find specific instance by uuid.
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Value> find(TableName table_name, std::string const& uuid);
  /// @brief List all the rows in the table.
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Document> scan(TableName table_name);
  /// @brief Add single model to table.
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Document> add(TableName table_name, DataModelConstRef data);
  // TODO - Document why I need `add` with an `std::initializer_list`.
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Document> add(TableName table_name, std::initializer_list<DataModelConstRef> data);
  // TODO - Document why `replace` and not `update`.
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Document> replace(TableName table_name, DataModelConstRef data);
  /// @brief Remove a model from table.
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Document> remove(TableName table_name, void* key);
};

}  // namespace watch_list_app::server::dal

#endif  // DATABASE_HPP_
