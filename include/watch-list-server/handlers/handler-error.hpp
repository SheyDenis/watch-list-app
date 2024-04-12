/**
 * @file handler-error.hpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#ifndef HANDLER_ERROR_HPP_
#define HANDLER_ERROR_HPP_

#include <httplib/httplib.h>

#include <optional>
#include <string>

#include "watch-list-server/http-utils.hpp"

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

std::string to_string(HandlerError::RequestInfo const& err);
std::string to_string(HandlerError const& err);

}  // namespace watch_list_app::server::handlers

#endif  // HANDLER_ERROR_HPP_
