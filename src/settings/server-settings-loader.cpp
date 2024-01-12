/**
 * @file server-settings-loader.cpp
 * @author denis
 * @brief
 * @date 12024-01-04
 *
 */

#include "watch-list-server/settings/server-settings-loader.hpp"

#include <fmt/core.h>

#include <memory>
#include <optional>
#include <variant>

#include "watch-list-server/dev-utils.hpp"
#include "watch-list-server/formatter-utils.hpp"
#include "watch-list-server/json/json-utils.hpp"
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
  LOG_NOT_IMPLEMENTED();
  if (!settings_json.IsObject()) {
    return ServerGenericError(fmt::format("Invalid settings JSON [{}]", settings_json.GetType()));
  }
  return std::nullopt;
}

}  // namespace watch_list_app::server::settings
