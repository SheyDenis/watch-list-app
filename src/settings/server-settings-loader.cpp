/**
 * @file server-settings-loader.cpp
 * @author denis
 * @brief
 * @date 12024-01-04
 *
 */

#include "watch-list-server/settings/server-settings-loader.hpp"

#include <fmt/core.h>
#include <rapidjson/schema.h>

#include <memory>
#include <optional>
#include <variant>

#include "watch-list-server/json/json-utils.hpp"
#include "watch-list-server/server-settings-schema.hpp"
#include "watch-list-server/settings/server-settings.hpp"

namespace watch_list_app::server::settings {

OptionalServerGenericError ServerSettingsLoader::load_settings(std::string const& settings_file) {
  if (ServerSettings::instance_ != nullptr) {
    return ServerGenericError(fmt::format("Called [{}] multiple times", __PRETTY_FUNCTION__));
  }

  auto res = json::JSONUtils::read_file(settings_file);
  if (std::holds_alternative<ServerGenericError>(res)) {
    return std::get<ServerGenericError>(res);
  }

  if (auto err = validate_settings(std::get<rapidjson::Document>(res))) {
    return err;
  }

  return ServerSettings::initialize(std::get<rapidjson::Document>(res));
}

OptionalServerGenericError ServerSettingsLoader::validate_settings(rapidjson::Document const& settings_json) {
  rapidjson::Document json_schema;
  if (json_schema.Parse(schema::SERVER_SETTINGS_SCHEMA).HasParseError()) {
    return ServerGenericError(
        fmt::format("Failed to load server settings schema [{}]", rapidjson::GetParseError_En(json_schema.GetParseError())));
  }
  rapidjson::SchemaDocument const schema(json_schema);
  rapidjson::SchemaValidator validator(schema);
  if (!settings_json.Accept(validator)) {
    return ServerGenericError(fmt::format("Server settings failed validation [{}]", json::JSONUtils::dump(validator.GetError())));
  }

  return std::nullopt;
}

}  // namespace watch_list_app::server::settings
