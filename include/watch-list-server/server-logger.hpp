/**
 * @file server-logger.hpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#ifndef SERVER_LOGGER_HPP_
#define SERVER_LOGGER_HPP_

#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <string>

namespace watch_list_app::server {

class ServerLogger : public spdlog::logger {
 private:
  static spdlog::level::level_enum default_level_;

 public:
  explicit ServerLogger(std::string const& name) : spdlog::logger(*spdlog::stderr_color_mt(name)) {}

  static void configure_logger(std::string const& root_logger_name, spdlog::level::level_enum level) {
    if (spdlog::get(root_logger_name) == nullptr) {
      spdlog::set_default_logger(spdlog::stderr_color_mt(root_logger_name));
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
};

}  // namespace watch_list_app::server

#endif  // SERVER_LOGGER_HPP_
