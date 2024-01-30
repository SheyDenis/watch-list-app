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

OptionalServerGenericError Database::initialize(settings::ServerSettingsDatabase const& settings) {
  if (instance_ != nullptr) {
    return ServerGenericError(fmt::format("Called [{}] multiple times", __PRETTY_FUNCTION__));
  }
  instance_.reset(new Database(settings.database_path));

  auto json_data = json::JSONUtils::read_file(instance_->database_path_);
  if (std::holds_alternative<ServerGenericError>(json_data)) {
    instance_.reset();
    return std::move(std::get<ServerGenericError>(json_data));
  }

  instance_->data_ = std::move(std::get<rapidjson::Document>(json_data));

  return std::nullopt;
}

ServerGenericErrorVariant<rapidjson::Value> Database::find(TableName table_name, std::string const& uuid) {
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

ServerGenericErrorVariant<rapidjson::Document> Database::scan(TableName table_name) {
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
}  // namespace watch_list_app::server::dal
