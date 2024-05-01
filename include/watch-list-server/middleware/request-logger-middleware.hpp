/**
 * @file request_logger_middleware.hpp
 * @author denis
 * @brief
 * @date 12024-04-13
 *
 */

#ifndef REQUEST_LOGGER_MIDDLEWARE_HPP_
#define REQUEST_LOGGER_MIDDLEWARE_HPP_

#include "watch-list-server/handlers/handler-base.hpp"
#include "watch-list-server/middleware/middleware-base.hpp"
#include "watch-list-server/pistache_utils.hpp"

namespace watch_list_app::server::middleware {

class RequestLoggerMiddleware : public MiddlewareBase {
 private:
  friend struct handlers::HandlerInstance<RequestLoggerMiddleware>;

  bool enabled_;

 public:
  RequestLoggerMiddleware() : MiddlewareBase("RequestLoggerMiddleware"), enabled_(true) {}

  void enable() {
    enabled_ = true;
  }
  void disable() {
    enabled_ = false;
  }

  bool log_request(HttpRequest& request, HttpResponseWriter& response) {
    if (enabled_) {
      // TODO - Move methodString to formatter-utils.
      logger_.info("Received [{}] request from [{}] to [{}]",
                   Pistache::Http::methodString(request.method()),
                   request.address().host(),
                   request.resource());
    }
    return true;
  }
};

}  // namespace watch_list_app::server::middleware

#endif  // REQUEST_LOGGER_MIDDLEWARE_HPP_
