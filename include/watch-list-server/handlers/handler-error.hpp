/**
 * @file handler-error.hpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#ifndef HANDLER_ERROR_HPP_
#define HANDLER_ERROR_HPP_

#include <fmt/format.h>
#include <spdlog/formatter.h>

#include <optional>
#include <string>

#include "watch-list-server/dev-utils.hpp"
#include "watch-list-server/http-utils.hpp"
#include "watch-list-server/server-constants.hpp"

namespace watch_list_app::server {

class HandlerError {
 private:
  HTTPMethod const method_;

 public:
  explicit HandlerError(HTTPMethod method) : method_(method) {}

  explicit operator std::string() const {
    LOG_NOT_IMPLEMENTED();
    if (ServerConstants::include_debug_data()) {
    }
    return fmt::format("Function [{}] is not implemented [{}]", __PRETTY_FUNCTION__, http_method_to_string(method_));
  }
};
typedef std::optional<HandlerError> OptionalHandlerError;

}  // namespace watch_list_app::server

template <>
struct fmt::formatter<watch_list_app::server::HandlerError> : fmt::formatter<std::string> {
  auto format(watch_list_app::server::HandlerError v, format_context& ctx) const -> decltype(ctx.out()) {
    return format_to(ctx.out(), std::string(v));
  }
};

#endif  // HANDLER_ERROR_HPP_
