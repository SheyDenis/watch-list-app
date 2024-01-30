/**
 * @file base-model.cpp
 * @author denis
 * @brief
 * @date 12024-01-29
 *
 */
#include "watch-list-server/models/base-model.hpp"

#include <rapidjson/allocators.h>
#include <rapidjson/encodings.h>

namespace watch_list_app::server::models {

void BaseModel::serialize(rapidjson::Value& output, rapidjson::Value::AllocatorType& allocator) const {
  serialize_impl(output, allocator);

  // TODO - Implement.
  output.AddMember(rapidjson::StringRef(BaseModel::CREATE_TIME), 123, allocator);
  output.AddMember(rapidjson::StringRef(BaseModel::MODIFY_TIME), 123, allocator);
}

bool BaseModel::deserialize(rapidjson::Value const& data) {
  if (!deserialize_impl(data)) {
    return false;
  }
  // TODO - Implement.
  //      create_time_ = data[BaseModel::CREATE_TIME].GetInt64();
  //      modify_time_ = data[BaseModel::MODIFY_TIME].GetInt64();

  return true;
}

}  // namespace watch_list_app::server::models
