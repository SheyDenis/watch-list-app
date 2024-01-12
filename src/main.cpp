/**
 * @file main.cpp
 * @author denis
 * @brief
 * @date 12023-12-30
 *
 */

#include <fmt/core.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <optional>
#include <string>

#include "watch-list-server/dev-utils.hpp"
#include "watch-list-server/formatter-utils.hpp"
#include "watch-list-server/server-constants.hpp"
#include "watch-list-server/server-error.hpp"
#include "watch-list-server/server-generic-error.hpp"
#include "watch-list-server/server-listener.hpp"
#include "watch-list-server/server-logger.hpp"
#include "watch-list-server/settings/server-settings-loader.hpp"
#include "watch-list-server/settings/server-settings-models.hpp"
#include "watch-list-server/settings/server-settings.hpp"

void show_help() {
  watch_list_app::server::LOG_NOT_IMPLEMENTED();
}

void configure_logger(spdlog::level::level_enum level = spdlog::level::level_enum::debug) {
  watch_list_app::server::ServerLogger::configure_logger(watch_list_app::server::ServerConstants::kRootLoggerName, level);
}

bool settings_path(int argc, char* argv[], std::string& output) {
  if (argc > 1) {
    output.assign(argv[1]);
    return true;
  }
  auto settings_file_path = watch_list_app::server::ServerConstants::get_settings_file_path();
  if (settings_file_path == nullptr) {
    return false;
  }
  output.assign(settings_file_path);
  return !output.empty();
}

watch_list_app::server::OptionalServerGenericError register_event_handlers() {
  watch_list_app::server::LOG_NOT_IMPLEMENTED();
  return std::nullopt;
}

int main(int argc, char* argv[]) {
  configure_logger();

  std::string settings_file_path;
  if (!settings_path(argc, argv, settings_file_path)) {
    show_help();
    return -1;
  }
  if (auto const err = watch_list_app::server::settings::ServerSettingsLoader::load_settings(settings_file_path)) {
    spdlog::error("Failed to load settings [{}]", *err);
    return -1;
  }
  configure_logger(watch_list_app::server::settings::ServerSettings::logging_settings().logger_level);

  watch_list_app::server::ServerListener listener;
  if (auto const err = register_event_handlers()) {
    spdlog::error("Failed to register event handlers [{}]", *err);
    return -1;
  }
  if (auto const err = listener.initialize()) {
    spdlog::error("Failed to initialize server [{}]", *err);
    return -1;
  }
  if (auto const err = listener.run()) {
    spdlog::error("Failed to run server [{}]", *err);
    return -1;
  }

  return 0;
}
