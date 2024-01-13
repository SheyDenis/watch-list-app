/**
 * @file server-constants.hpp
 * @author denis
 * @brief
 * @date 12024-01-04
 *
 */

#ifndef SERVER_CONSTANTS_HPP_
#define SERVER_CONSTANTS_HPP_

#include <spdlog/spdlog.h>

#include <cstdlib>

namespace watch_list_app::server {

class ServerConstants {
 public:
  static auto constexpr kRootLoggerName = "root";
  static auto constexpr kSettingsFilePathEnvVar = "SETTINGS_FILE_PATH";

 private:
  template <typename T>
  static T get_env_var(char const* name, T default_value) {
    auto const res = std::getenv(name);
    if (res != nullptr) {
      return T(res);
    }
    return default_value;
  }

 public:
  static spdlog::logger& root_logger() {
    return *spdlog::get(kRootLoggerName);
  }

  static bool include_debug_data() {
    switch (spdlog::get_level()) {
      case spdlog::level::level_enum::trace:
      case spdlog::level::level_enum::debug:
        return true;
      default:
        return false;
    }
  }

  static char const* get_settings_file_path() {
    return get_env_var<char const*>(kSettingsFilePathEnvVar, nullptr);
  }
};

}  // namespace watch_list_app::server

#endif  // SERVER_CONSTANTS_HPP_
