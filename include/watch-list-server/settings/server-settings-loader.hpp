/**
 * @file server-settings-loader.hpp
 * @author denis
 * @brief
 * @date 12024-01-04
 *
 */

#ifndef SERVER_SETTINGS_LOADER_HPP_
#define SERVER_SETTINGS_LOADER_HPP_

#include <rapidjson/document.h>
#include <spdlog/common.h>

#include <memory>
#include <string>

#include "watch-list-server/server-generic-error.hpp"

namespace watch_list_app::server::settings {

class ServerSettingsLoader {
 private:
  static OptionalServerGenericError validate_settings(rapidjson::Document const& settings_json);

 public:
  static OptionalServerGenericError load_settings(std::string const& settings_file);
};

}  // namespace watch_list_app::server::settings

#endif  // SERVER_SETTINGS_LOADER_HPP_
