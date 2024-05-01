/**
 * @file server-logger.hpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#ifndef SERVER_LOGGER_HPP_
#define SERVER_LOGGER_HPP_

#include <pistache/string_logger.h>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <string>

namespace watch_list_app::server {

class ServerLogger : public spdlog::logger {
 public:
  explicit ServerLogger(std::string const& name)
      : spdlog::logger(spdlog::get(name) != nullptr ? *spdlog::get(name) : *spdlog::stderr_color_mt(name)) {}

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

class PistacheServerLogger : public ServerLogger, public Pistache::Log::StringLogger {
  // TODO - Implement all required operators. (copy and move operators/constructors)
 private:
  static spdlog::level::level_enum pistache_level_to_spdlog_level(Pistache::Log::Level level) noexcept {
    switch (level) {
      case Pistache::Log::Level::TRACE:
        return spdlog::level::trace;
      case Pistache::Log::Level::DEBUG:
        return spdlog::level::debug;
      case Pistache::Log::Level::INFO:
        return spdlog::level::info;
      case Pistache::Log::Level::WARN:
        return spdlog::level::warn;
      case Pistache::Log::Level::ERROR:
        return spdlog::level::err;
      case Pistache::Log::Level::FATAL:
        return spdlog::level::critical;
    }
    __builtin_unreachable();
  }

 public:
  explicit PistacheServerLogger(std::string const& name) : ServerLogger(name) {}
  ~PistacheServerLogger() override = default;

  void log(Pistache::Log::Level level, std::string const& message) override {
    ServerLogger::log(pistache_level_to_spdlog_level(level), message);
  }

  bool isEnabledFor(Pistache::Log::Level level) const override {
    return ServerLogger::should_log(pistache_level_to_spdlog_level(level));
  }
};

}  // namespace watch_list_app::server

#endif  // SERVER_LOGGER_HPP_
