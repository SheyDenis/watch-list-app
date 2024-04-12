/**
 * @file main.cpp
 * @author denis
 * @brief
 * @date 12023-12-30
 *
 */

#include <fmt/core.h>
#include <spdlog/common.h>
#include <spdlog/logger.h>

#include <cstdio>
#include <string>

#include "watch-list-server/server-constants.hpp"
#include "watch-list-server/server-error.hpp"
#include "watch-list-server/server-generic-error.hpp"
#include "watch-list-server/server-listener.hpp"
#include "watch-list-server/server-logger.hpp"
#include "watch-list-server/settings/server-settings-loader.hpp"
#include "watch-list-server/settings/server-settings-models.hpp"
#include "watch-list-server/settings/server-settings.hpp"

void show_help(char const* executable) {
  std::string binary_file(executable);
  auto const sep = binary_file.rfind('/');
  if (sep != std::string::npos) {
    binary_file = binary_file.substr(sep + 1);
  }
  fmt::println(stderr, "Usage:\n\t./{} <path_to_settings_file>", binary_file);
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

int main(int argc, char* argv[]) {
  configure_logger();

  auto& logger = watch_list_app::server::ServerConstants::root_logger();
  std::string settings_file_path;
  if (!settings_path(argc, argv, settings_file_path)) {
    show_help(argv[0]);
    return -1;
  }
  if (auto const err = watch_list_app::server::settings::ServerSettingsLoader::load_settings(settings_file_path)) {
    logger.error("Failed to load settings [{}]", watch_list_app::server::format_error(err));
    return -1;
  }
  configure_logger(watch_list_app::server::settings::ServerSettings::logging_settings().logger_level);

  watch_list_app::server::ServerListener listener;
  if (auto const err = listener.initialize()) {
    logger.error("Failed to initialize server [{}]", watch_list_app::server::format_error(err));
    return -1;
  }
  if (auto const err = listener.run()) {
    logger.error("Failed to run server [{}]", watch_list_app::server::format_error(err));
    return -1;
  }

  return 0;
}
