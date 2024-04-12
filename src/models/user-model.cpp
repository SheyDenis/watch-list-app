/**
 * @file user-model.cpp
 * @author denis
 * @brief
 * @date 12024-01-18
 *
 */

#include "watch-list-server/models/user-model.hpp"

#include <rapidjson/allocators.h>
#include <rapidjson/rapidjson.h>

#include <algorithm>
#include <optional>
#include <stdexcept>
#include <utility>

#include "watch-list-server/dev-utils.hpp"
namespace {

[[maybe_unused]] rapidjson::Document const& get_model_schema() {
  static bool initialized = false;
  static rapidjson::Document json_schema(rapidjson::kObjectType);

  if (!initialized) {
    if (json_schema
            .Parse(
                R"(
{
  "$schema": "https://json-schema.org/draft/2020-12/schema",
  "type": "object",
  "additionalProperties": true
}
)"  // TODO - Implement.
                )
            .HasParseError()) {
      throw std::runtime_error("Failed to load UserModel JSON schema");
    }

    initialized = true;
  }

  return json_schema;
}

}  // namespace

namespace watch_list_app::server::models {

bool UserModel::is_valid_email_address(std::string const& value) {
  // TODO - Implement validating email address.
  return std::count(value.begin(), value.end(), '@') == 1;
}

bool UserModel::is_valid_password(SecureString const& value) {
  // TODO - Implement validating password rules.
  return value.length() >= 4;
}

UserModel::PasswdHash UserModel::hash_passwd(SecureString const& passwd) {
  LOG_NOT_IMPLEMENTED();
  // TODO - Implement.
  return passwd;
}

UserModel::UserModel() : BaseModel(), uuid_(uuids::uuid_system_generator{}()), name_({}), email_({}), passwd_hash_({}), roles_({}) {}

uuids::uuid const& UserModel::get_uuid() const {
  return uuid_;
}

std::string const& UserModel::get_name() const {
  return name_;
}

void UserModel::set_name(std::string name) {
  name_ = std::move(name);
}

std::string const& UserModel::get_email() const {
  return email_;
}

bool UserModel::set_email(std::string email) {
  if (!UserModel::is_valid_email_address(email)) {
    return false;
  }
  email_ = std::move(email);
  return true;
}

bool UserModel::set_passwd(SecureString const& passwd) {
  if (!UserModel::is_valid_password(passwd)) {
    return false;
  }
  passwd_hash_ = UserModel::hash_passwd(passwd);
  return true;
}

bool UserModel::check_passwd(SecureString const& passwd) const {
  // TODO - Implement.
  return UserModel::hash_passwd(passwd) == passwd_hash_;
}

std::set<std::string> const& UserModel::get_roles() const {
  return roles_;
}

void UserModel::add_roles(std::initializer_list<std::string> roles) {
  for (auto const& itr : roles) {
    roles_.insert(itr);
  }
}

void UserModel::remove_roles(std::initializer_list<std::string> roles) {
  for (auto const& itr : roles) {
    roles_.erase(itr);
  }
}

void UserModel::serialize_impl(rapidjson::Value& output, rapidjson::Value::AllocatorType& allocator) const {
  output.SetObject();

  std::string uuid_str(uuids::to_string(uuid_));

  output.AddMember("uuid", rapidjson::Value(uuid_str.c_str(), allocator).Move(), allocator);
  output.AddMember("name", rapidjson::Value(name_.c_str(), allocator).Move(), allocator);
  output.AddMember("email", rapidjson::Value(email_.c_str(), allocator).Move(), allocator);

  rapidjson::Value roles_value(rapidjson::kArrayType);
  for (auto const& itr : roles_) {
    rapidjson::Value v(rapidjson::kStringType);
    v.SetString(itr.c_str(), allocator);
    roles_value.PushBack(v.Move(), allocator);
  }
  output.AddMember("roles", roles_value.Move(), allocator);
}

bool UserModel::deserialize_impl(rapidjson::Value const& data) {
  if (data.HasMember("uuid")) {
    auto tmp_uuid = uuids::uuid::from_string(data["uuid"].GetString());
    if (!tmp_uuid.has_value()) {
      return false;
    }
    uuid_ = *tmp_uuid;
  } else {
    uuid_ = uuids::uuid_system_generator{}();
  }

  if (!UserModel::is_valid_email_address(data["email"].GetString())) {
    return false;
  }

  name_ = data["name"].GetString();
  email_ = data["email"].GetString();

  passwd_hash_ = data["passwd_hash"].GetString();

  if (data.HasMember("roles")) {
    for (auto const& itr : data["roles"].GetArray()) {
      roles_.insert(itr.GetString());
    }
  }

  return true;
}

}  // namespace watch_list_app::server::models
