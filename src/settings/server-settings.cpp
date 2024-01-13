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
  if (settings_json.HasMember("logging")) {
    initialize_logging_settings(settings_json["logging"].GetObject());
  }
  if (settings_json.HasMember("server")) {
    initialize_server_settings(settings_json["server"].GetObject());
  }
  if (settings_json.HasMember("httplib")) {
    initialize_httplib_settings(settings_json["httplib"].GetObject());
  }
}

void ServerSettings::initialize_server_settings(rapidjson::Value::ConstObject const& settings_json) {
  if (settings_json.HasMember("server_address")) {
    server_settings_.server_address = settings_json["server_address"].GetString();
  }
  if (settings_json.HasMember("server_port")) {
    server_settings_.server_port = settings_json["server_port"].GetInt();
  }
}

void ServerSettings::initialize_httplib_settings(rapidjson::Value::ConstObject const& settings_json) {
  if (settings_json.HasMember("thread_pool_size")) {
    httplib_settings_.thread_pool_size = settings_json["thread_pool_size"].GetInt();
  }
  if (settings_json.HasMember("keep_alive_max_count")) {
    httplib_settings_.keep_alive_max_count = settings_json["keep_alive_max_count"].GetInt64();
  }
  if (settings_json.HasMember("keep_alive_timeout_sec")) {
    httplib_settings_.keep_alive_timeout_sec = settings_json["keep_alive_timeout_sec"].GetInt64();
  }
  if (settings_json.HasMember("read_timeout_sec")) {
    httplib_settings_.read_timeout_sec = settings_json["read_timeout_sec"].GetInt64();
  }
  if (settings_json.HasMember("read_timeout_usec")) {
    httplib_settings_.read_timeout_usec = settings_json["read_timeout_usec"].GetInt64();
  }
  if (settings_json.HasMember("write_timeout_sec")) {
    httplib_settings_.write_timeout_sec = settings_json["write_timeout_sec"].GetInt64();
  }
  if (settings_json.HasMember("write_timeout_usec")) {
    httplib_settings_.write_timeout_usec = settings_json["write_timeout_usec"].GetInt64();
  }
  if (settings_json.HasMember("idle_interval_sec")) {
    httplib_settings_.idle_interval_sec = settings_json["idle_interval_sec"].GetInt64();
  }
}

void ServerSettings::initialize_logging_settings(rapidjson::Value::ConstObject const& settings_json) {
  if (settings_json.HasMember("level")) {
    logging_settings_.logger_level = spdlog::level::from_str(settings_json["level"].GetString());
  }
}

}  // namespace watch_list_app::server::settings
