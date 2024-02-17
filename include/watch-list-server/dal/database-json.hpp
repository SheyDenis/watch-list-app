/**
 * @file database-json.hpp
 * @author denis
 * @brief
 * @date 12024-02-12
 *
 */

#ifndef DATABASE_JSON_HPP_
#define DATABASE_JSON_HPP_

#include <rapidjson/document.h>

#include <optional>
#include <variant>

#include "watch-list-server/dal/database.hpp"
#include "watch-list-server/server-generic-error.hpp"

namespace watch_list_app::server::dal {

class DatabaseJSON : public Database {
 private:
  std::string database_path_;
  rapidjson::Document data_;

  friend class DatabaseBuilder;

 protected:
  explicit DatabaseJSON(std::string database_path) noexcept(false);

  [[nodiscard]] OptionalServerGenericError initialize_impl() override;
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Value> find_impl(TableName table_name, std::string const& uuid) override;
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Document> scan_impl(TableName table_name) override;
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Document> add_impl(TableName table_name, DataModelConstRef data) override;
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Document> add_impl(TableName table_name,
                                                                        std::initializer_list<DataModelConstRef> data) override;
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Document> replace_impl(TableName table_name, DataModelConstRef data) override;
  [[nodiscard]] ServerGenericErrorVariant<rapidjson::Document> remove_impl(TableName table_name, void* key) override;

 public:
  ~DatabaseJSON() override = default;
};

}  // namespace watch_list_app::server::dal

#endif  // DATABASE_JSON_HPP_
