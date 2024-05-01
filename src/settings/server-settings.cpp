/**
 * @file server-settings.cpp
 * @author denis
 * @brief
 * @date 12024-01-04
 *
 */

#include "watch-list-server/settings/server-settings.hpp"

#include <fmt/core.h>
#include <rapidjson/allocators.h>
#include <spdlog/common.h>

#include <optional>
#include <stdexcept>

namespace watch_list_app::server::settings {

std::unique_ptr<ServerSettings> ServerSettings::instance_ = nullptr;

OptionalServerGenericError ServerSettings::initialize(rapidjson::Document const& settings_json) {
  if (instance_ != nullptr) {
    return ServerGenericError(fmt::format("Called [{}] multiple times", __PRETTY_FUNCTION__));
  }

  instance_.reset(new ServerSettings(settings_json));
  return std::nullopt;
}

ServerSettings::ServerSettings(rapidjson::Document const& settings_json)
    : server_settings_({}), logging_settings_({}), pistache_settings_({}), database_settings_({}) {
  if (settings_json.HasMember("logging")) {
    initialize_logging_settings(settings_json["logging"].GetObject());
  }
  if (settings_json.HasMember("server")) {
    initialize_server_settings(settings_json["server"].GetObject());
  }
  if (settings_json.HasMember("pistache")) {
    initialize_pistache_settings(settings_json["pistache"].GetObject());
  }
  if (settings_json.HasMember("database")) {
    initialize_database_settings(settings_json["database"].GetObject());
  }
}

void ServerSettings::initialize_server_settings(rapidjson::Value::ConstObject const& settings_json) {
  if (settings_json.HasMember("server_address")) {
    server_settings_.server_address = settings_json["server_address"].GetString();
  }
  if (settings_json.HasMember("server_port")) {
    server_settings_.server_port = static_cast<uint16_t>(settings_json["server_port"].GetUint());
  }
  if (settings_json.HasMember("log_requests")) {
    server_settings_.log_requests = settings_json["log_requests"].GetBool();
  }
}

void ServerSettings::initialize_pistache_settings(rapidjson::Value::ConstObject const& settings_json) {
  if (settings_json.HasMember("max_backlog")) {
    pistache_settings_.max_backlog = settings_json["max_backlog"].GetInt();
  }
  //  if (settings_json.HasMember("max_events")) {
  //    pistache_settings_.max_events = settings_json["max_events"].GetUint64();
  //  }
  //  if (settings_json.HasMember("max_buffer")) {
  //    pistache_settings_.max_buffer = settings_json["max_buffer"].GetUint64();
  //  }
  //  if (settings_json.HasMember("workers")) {
  //    pistache_settings_.workers = settings_json["workers"].GetUint64();
  //  }
  //  if (settings_json.HasMember("timer_pool_size")) {
  //    pistache_settings_.timer_pool_size = settings_json["timer_pool_size"].GetUint64();
  //  }
  if (settings_json.HasMember("max_request_size")) {
    pistache_settings_.max_request_size = settings_json["max_request_size"].GetUint64();
  }
  if (settings_json.HasMember("max_response_size")) {
    pistache_settings_.max_response_size = settings_json["max_response_size"].GetUint64();
  }
  if (settings_json.HasMember("header_timeout_sec")) {
    pistache_settings_.header_timeout = std::chrono::seconds(settings_json["header_timeout_sec"].GetInt64());
  }
  if (settings_json.HasMember("keep_alive_timeout_sec")) {
    pistache_settings_.keep_alive_timeout = std::chrono::seconds(settings_json["keep_alive_timeout_sec"].GetInt64());
  }
  if (settings_json.HasMember("ssl_handshake_timeout_sec")) {
    pistache_settings_.ssl_handshake_timeout = std::chrono::seconds(settings_json["ssl_handshake_timeout_sec"].GetInt64());
  }
  //  if (settings_json.HasMember("chunk_size")) {
  //    pistache_settings_.chunk_size = settings_json["chunk_size"].GetUint64();
  //  }
}

void ServerSettings::initialize_logging_settings(rapidjson::Value::ConstObject const& settings_json) {
  if (settings_json.HasMember("level")) {
    logging_settings_.logger_level = spdlog::level::from_str(settings_json["level"].GetString());
  }
}

void ServerSettings::initialize_database_settings(rapidjson::Value::ConstObject const& settings_json) {
  if (settings_json.HasMember("database_type")) {
    auto const& database_type = settings_json["database_type"];
    if (database_type == "json") {
      database_settings_.database_type = ServerSettingsDatabase::DatabaseType::JSON;
    } else {
      throw std::runtime_error("Invalid database type");
    }
  }
  if (settings_json.HasMember("database_path")) {
    database_settings_.database_path = settings_json["database_path"].GetString();
  }
}

}  // namespace watch_list_app::server::settings
