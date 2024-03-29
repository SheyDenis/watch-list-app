/**
 * @file server-settings.hpp
 * @author denis
 * @brief
 * @date 12024-01-04
 *
 */

#ifndef SERVER_SETTINGS_HPP_
#define SERVER_SETTINGS_HPP_

#include <rapidjson/document.h>

#include <memory>

#include "watch-list-server/server-generic-error.hpp"
#include "watch-list-server/settings/server-settings-models.hpp"

namespace watch_list_app::server::settings {

class ServerSettings {
 private:
  static std::unique_ptr<ServerSettings> instance_;

  ServerSettingsServer server_settings_;
  ServerSettingsLogging logging_settings_;
  ServerSettingsHTTPLib httplib_settings_;

 private:
  explicit ServerSettings(rapidjson::Document const& settings_json);

  [[nodiscard]] static OptionalServerGenericError initialize(rapidjson::Document const& settings_json);

  void initialize_server_settings(rapidjson::Value::ConstObject const& settings_json);
  void initialize_logging_settings(rapidjson::Value::ConstObject const& settings_json);
  void initialize_httplib_settings(rapidjson::Value::ConstObject const& settings_json);

  friend class ServerSettingsLoader;

 public:
  [[nodiscard]] static auto const& server_settings() {
    return instance_->server_settings_;
  }
  [[nodiscard]] static auto const& httplib_settings() {
    return instance_->httplib_settings_;
  }
  [[nodiscard]] static auto const& logging_settings() {
    return instance_->logging_settings_;
  }
};

}  // namespace watch_list_app::server::settings

#endif  // SERVER_SETTINGS_HPP_
