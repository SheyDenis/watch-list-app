/**
 * @file main.cpp
 * @author denis
 * @brief
 * @date 12023-12-30
 *
 */

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <string>

#include "watch-list-server/dev-utils.hpp"
#include "watch-list-server/server-constants.hpp"
#include "watch-list-server/settings/server-settings-loader.hpp"
#include "watch-list-server/settings/server-settings.hpp"

void show_help() {
  watch_list_app::server::LOG_NOT_IMPLEMENTED();
}

void configure_logger(spdlog::level::level_enum level = spdlog::level::level_enum::debug) {
  if (spdlog::get(watch_list_app::server::ServerConstants::kRootLoggerName) == nullptr) {
    spdlog::set_default_logger(spdlog::stderr_color_mt(watch_list_app::server::ServerConstants::kRootLoggerName));
  }
  switch (level) {
    case spdlog::level::level_enum::trace:
    case spdlog::level::level_enum::debug:
      spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e][%l][%n][%!][%t] %v");
      break;
    default:
      spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e][%l][%n][%t] %v");
  }
  spdlog::set_level(level);
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

int main(int argc, char* argv[]) {
  configure_logger();

  std::string settings_file_path;
  if (!settings_path(argc, argv, settings_file_path)) {
    show_help();
    exit(-1);
  }
  if (auto const err = watch_list_app::server::settings::ServerSettingsLoader::load_settings(settings_file_path)) {
    spdlog::error("Failed to load settings [{}]", std::string(*err));
  }
  configure_logger(watch_list_app::server::settings::ServerSettings::logging_settings().logger_level);

  return 0;
}
