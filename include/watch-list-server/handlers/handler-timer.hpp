/**
 * @file handler-timer.hpp
 * @author denis
 * @brief
 * @date 12024-01-13
 *
 */

#ifndef HANDLER_TIMER_HPP_
#define HANDLER_TIMER_HPP_

#include <chrono>
#include <string>

#include "watch-list-server/http-utils.hpp"
#include "watch-list-server/server-logger.hpp"

namespace watch_list_app::server {
class HandlerTimer {
 private:
  ServerLogger logger_;
  std::string const& handler_name_;
  HTTPMethod method_;
  std::chrono::system_clock::time_point start_time_;

 public:
  HandlerTimer(std::string const& handler_name, HTTPMethod method)
      : logger_("HandlerTimer"), handler_name_(handler_name), method_(method), start_time_(std::chrono::system_clock::now()) {}

  ~HandlerTimer() {
    logger_.debug("Handler [{}][{}] completed in [{}] microseconds",
                  handler_name_,
                  http_method_to_string(method_),
                  std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start_time_).count());
  }
};
}  // namespace watch_list_app::server

#endif  // HANDLER_TIMER_HPP_
