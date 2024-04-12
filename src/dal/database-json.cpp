/**
 * @file database-json.cpp
 * @author denis
 * @brief
 * @date 12024-02-12
 *
 */

#include "watch-list-server/dal/database-json.hpp"

#include <stdexcept>
#include <utility>

#include "watch-list-server/dev-utils.hpp"
#include "watch-list-server/json/json-utils.hpp"
#include "watch-list-server/server-generic-error.hpp"

namespace watch_list_app::server::dal {
DatabaseJSON::DatabaseJSON(std::string database_path)
    : Database("DatabaseJSON"), database_path_(std::move(database_path)), data_(rapidjson::kNullType) {}

OptionalServerGenericError DatabaseJSON::initialize_impl() {
  auto json_data = json::JSONUtils::read_file(database_path_);
  if (std::holds_alternative<ServerGenericError>(json_data)) {
    return std::get<ServerGenericError>(json_data);
  }

  data_ = std::move(std::get<rapidjson::Document>(json_data));
  return std::nullopt;
}

ServerGenericErrorVariant<rapidjson::Value> DatabaseJSON::find_impl(TableName table_name, std::string const& uuid) {
  if (!data_.HasMember(table_name.data())) {
    return ServerGenericError(fmt::format("No such table [{}]", table_name));
  }
  for (auto const& itr : data_[table_name.data()].GetArray()) {
    if (itr["uuid"].GetString() == uuid) {
      rapidjson::Value res;
      res.CopyFrom(itr, data_.GetAllocator(), true);
      return res;
    }
  }

  return rapidjson::Value(rapidjson::kNullType);
}

ServerGenericErrorVariant<rapidjson::Document> DatabaseJSON::scan_impl(TableName table_name) {
  if (!data_.HasMember(table_name.data())) {
    return ServerGenericError(fmt::format("No such table [{}]", table_name));
  }

  rapidjson::Document res(rapidjson::kArrayType);

  for (auto const& itr : data_[table_name.data()].GetArray()) {
    rapidjson::Value v;
    v.CopyFrom(itr, res.GetAllocator(), true);
    res.PushBack(v, res.GetAllocator());
  }
  return res;
}

ServerGenericErrorVariant<rapidjson::Document> DatabaseJSON::add_impl([[maybe_unused]] TableName table_name,
                                                                      [[maybe_unused]] DataModelConstRef data) {
  LOG_NOT_IMPLEMENTED();
  throw std::runtime_error(fmt::format("Method [{}] is not implemented", __PRETTY_FUNCTION__));
}

ServerGenericErrorVariant<rapidjson::Document> DatabaseJSON::add_impl([[maybe_unused]] TableName table_name,
                                                                      [[maybe_unused]] std::initializer_list<DataModelConstRef> data) {
  LOG_NOT_IMPLEMENTED();
  throw std::runtime_error(fmt::format("Method [{}] is not implemented", __PRETTY_FUNCTION__));
}

ServerGenericErrorVariant<rapidjson::Document> DatabaseJSON::replace_impl([[maybe_unused]] TableName table_name,
                                                                          [[maybe_unused]] DataModelConstRef data) {
  LOG_NOT_IMPLEMENTED();
  throw std::runtime_error(fmt::format("Method [{}] is not implemented", __PRETTY_FUNCTION__));
}

ServerGenericErrorVariant<rapidjson::Document> DatabaseJSON::remove_impl([[maybe_unused]] TableName table_name,
                                                                         [[maybe_unused]] void* key) {
  LOG_NOT_IMPLEMENTED();
  throw std::runtime_error(fmt::format("Method [{}] is not implemented", __PRETTY_FUNCTION__));
}

}  // namespace watch_list_app::server::dal
