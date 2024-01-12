/**
 * @file server-settings-models.hpp
 * @author denis
 * @brief
 * @date 12024-01-05
 *
 */

#ifndef SERVER_SETTINGS_MODELS_HPP_
#define SERVER_SETTINGS_MODELS_HPP_

#include <spdlog/common.h>

#include <string>

namespace watch_list_app::server::settings {

struct ServerSettingsBase {};

struct ServerSettingsServer : public ServerSettingsBase {
  std::string server_address = "localhost";
  int server_port = 8000;
};

struct ServerSettingsLogging : public ServerSettingsBase {
  spdlog::level::level_enum logger_level = spdlog::level::level_enum::info;
};

}  // namespace watch_list_app::server::settings

#endif  // SERVER_SETTINGS_MODELS_HPP_
