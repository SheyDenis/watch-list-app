/**
 * @file server-settings-models.hpp
 * @author denis
 * @brief
 * @date 12024-01-05
 *
 */

#ifndef SERVER_SETTINGS_MODELS_HPP_
#define SERVER_SETTINGS_MODELS_HPP_

#include <httplib/httplib.h>
#include <spdlog/common.h>

#include <string>

namespace watch_list_app::server::settings {

struct ServerSettingsBase {};

struct ServerSettingsServer : public ServerSettingsBase {
  std::string server_address = "localhost";
  int server_port = 8000;
};

struct ServerSettingsHTTPLib : public ServerSettingsBase {
  std::size_t thread_pool_size = CPPHTTPLIB_THREAD_POOL_COUNT;
  std::size_t payload_max_length = CPPHTTPLIB_PAYLOAD_MAX_LENGTH;
  std::size_t keep_alive_max_count = CPPHTTPLIB_KEEPALIVE_MAX_COUNT;
  std::time_t keep_alive_timeout_sec = CPPHTTPLIB_KEEPALIVE_TIMEOUT_SECOND;
  std::time_t read_timeout_sec = CPPHTTPLIB_READ_TIMEOUT_SECOND;
  std::time_t read_timeout_usec = CPPHTTPLIB_READ_TIMEOUT_USECOND;
  std::time_t write_timeout_sec = CPPHTTPLIB_WRITE_TIMEOUT_SECOND;
  std::time_t write_timeout_usec = CPPHTTPLIB_WRITE_TIMEOUT_USECOND;
  std::time_t idle_interval_sec = CPPHTTPLIB_IDLE_INTERVAL_SECOND;
  std::time_t idle_interval_usec = CPPHTTPLIB_IDLE_INTERVAL_USECOND;
};

struct ServerSettingsLogging : public ServerSettingsBase {
  spdlog::level::level_enum logger_level = spdlog::level::level_enum::info;
};

}  // namespace watch_list_app::server::settings

#endif  // SERVER_SETTINGS_MODELS_HPP_
