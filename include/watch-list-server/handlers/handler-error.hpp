/**
 * @file handler-error.hpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#ifndef HANDLER_ERROR_HPP_
#define HANDLER_ERROR_HPP_

#include <fmt/core.h>
#include <httplib/httplib.h>
#include <spdlog/formatter.h>

#include <optional>
#include <string>

#include "watch-list-server/formatter-utils.hpp"
#include "watch-list-server/http-utils.hpp"
#include "watch-list-server/server-constants.hpp"

namespace watch_list_app::server::handlers {

struct HandlerError {
  /// @brief Represents an error while handling a request.

  struct RequestInfo {
    int const return_code;
    httplib::Params const url_parameters;

    RequestInfo(int _return_code, httplib::Params _url_parameters)
        : return_code(_return_code), url_parameters(std::move(_url_parameters)) {}
  };

  HTTPMethod const method;
  std::string const handler_name;
  RequestInfo const request_info;
  std::string const error;
  std::optional<std::string> const ex;

  HandlerError(HTTPMethod _method, std::string _handler_name, RequestInfo _request_info, std::string _error, std::exception const& _ex)
      : method(_method),
        handler_name(std::move(_handler_name)),
        request_info(std::move(_request_info)),
        error(std::move(_error)),
        ex(_ex.what()) {}
  HandlerError(HTTPMethod _method, std::string _handler_name, RequestInfo _request_info, std::string _error)
      : method(_method),
        handler_name(std::move(_handler_name)),
        request_info(std::move(_request_info)),
        error(std::move(_error)),
        ex(std::nullopt) {}
};
using OptionalHandlerError = std::optional<HandlerError>;

static std::string to_string(HandlerError::RequestInfo const& err) {
  return fmt::format(FMT_STRING("[return_code={:d}][url_parameters={:s}]"), err.return_code, err.url_parameters);
}

static std::string to_string(HandlerError const& err) {
  std::string error_msg(fmt::format(
      FMT_STRING("Handler [{}] failed [{}] request [error={}]"), err.handler_name, http_method_to_string(err.method), err.error));
  if (ServerConstants::include_debug_data()) {
    error_msg.append(
        fmt::format(FMT_STRING("[ex={}][request_info={}]"), err.ex.has_value() ? *err.ex : "N/A", to_string(err.request_info)));
  }
  return error_msg;
}

}  // namespace watch_list_app::server::handlers

#endif  // HANDLER_ERROR_HPP_
