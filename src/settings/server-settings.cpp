/**
 * @file server-settings.cpp
 * @author denis
 * @brief
 * @date 12024-01-04
 *
 */

#include "watch-list-server/settings/server-settings.hpp"

#include <fmt/core.h>
#include <spdlog/common.h>

#include <cstring>
#include <optional>

namespace watch_list_app::server::settings {

std::unique_ptr<ServerSettings> ServerSettings::instance_ = nullptr;

OptionalServerGenericError ServerSettings::initialize(rapidjson::Document const& settings_json) {
  if (instance_ != nullptr) {
    return ServerGenericError(fmt::format("Called [{}] multiple times", __PRETTY_FUNCTION__));
  }

  instance_.reset(new ServerSettings(settings_json));
  return std::nullopt;
}

ServerSettings::ServerSettings(rapidjson::Document const& settings_json) {
  if (settings_json["logging"].HasMember("level")) {
    logging_settings_.logger_level = strcmp(settings_json["logging"]["level"].GetString(), "debug") == 0 ? spdlog::level::level_enum::debug
                                                                                                         : spdlog::level::level_enum::info;
  }
}

}  // namespace watch_list_app::server::settings
