/**
 * @file server-settings-models.hpp
 * @author denis
 * @brief
 * @date 12024-01-05
 *
 */

#ifndef SERVER_SETTINGS_MODELS_HPP_
#define SERVER_SETTINGS_MODELS_HPP_

#include <pistache/config.h>
#include <spdlog/common.h>

#include <string>
#include <chrono>

namespace watch_list_app::server::settings {

struct ServerSettingsBase {};

struct ServerSettingsServer : public ServerSettingsBase {
  std::string server_address = "localhost";
  std::uint16_t server_port = 8000;
  bool log_requests = false;
};

struct ServerSettingsPistache : public ServerSettingsBase {
  int max_backlog = Pistache::Const::MaxBacklog;
//  std::size_t max_events = Pistache::Const::MaxEvents;
//  std::size_t max_buffer = Pistache::Const::MaxBuffer;
//  std::size_t workers = Pistache::Const::DefaultWorkers;
//  std::size_t timer_pool_size = Pistache::Const::DefaultTimerPoolSize;
  std::size_t max_request_size = Pistache::Const::DefaultMaxRequestSize;
  std::size_t max_response_size = Pistache::Const::DefaultMaxResponseSize;
  std::chrono::seconds header_timeout = Pistache::Const::DefaultHeaderTimeout;
  std::chrono::seconds keep_alive_timeout = Pistache::Const::DefaultKeepaliveTimeout;
  std::chrono::seconds ssl_handshake_timeout = Pistache::Const::DefaultSSLHandshakeTimeout;
//  std::size_t chunk_size = Pistache::Const::ChunkSize;
};

struct ServerSettingsLogging : public ServerSettingsBase {
  spdlog::level::level_enum logger_level = spdlog::level::level_enum::info;
};

struct ServerSettingsDatabase : public ServerSettingsBase {
  enum class DatabaseType {
    JSON = 0,
  };
  DatabaseType database_type = DatabaseType::JSON;
  std::string database_path = "database/database.json";
};

}  // namespace watch_list_app::server::settings

#endif  // SERVER_SETTINGS_MODELS_HPP_
