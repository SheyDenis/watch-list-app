/**
 * @file handler-error.hpp
 * @author denis
 * @brief
 * @date 12024-02-17
 *
 */

#include "watch-list-server/handlers/handler-error.hpp"

#include <fmt/core.h>
#include <spdlog/formatter.h>

#include "watch-list-server/formatter-utils.hpp"
#include "watch-list-server/server-constants.hpp"

namespace watch_list_app::server::handlers {

std::string to_string(HandlerError::RequestInfo const& err) {
  return fmt::format(FMT_STRING("[return_code={:d}][url_parameters={:s}]"), err.return_code, err.url_parameters);
}

std::string to_string(HandlerError const& err) {
  std::string error_msg(fmt::format(
      FMT_STRING("Handler [{}] failed [{}] request [error={}]"), err.handler_name, http_method_to_string(err.method), err.error));
  if (ServerConstants::include_debug_data()) {
    error_msg.append(
        fmt::format(FMT_STRING("[ex={}][request_info={}]"), err.ex.has_value() ? *err.ex : "N/A", to_string(err.request_info)));
  }
  return error_msg;
}

}  // namespace watch_list_app::server::handlers
