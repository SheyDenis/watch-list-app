/**
 * @file handler-base.hpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#ifndef HANDLER_BASE_HPP_
#define HANDLER_BASE_HPP_

#include <httplib/httplib.h>

#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "watch-list-server/handlers/handler-error.hpp"
#include "watch-list-server/handlers/handler-timer.hpp"
#include "watch-list-server/handlers/handler-traits.hpp"
#include "watch-list-server/http-utils.hpp"
#include "watch-list-server/server-generic-error.hpp"
#include "watch-list-server/server-logger.hpp"

namespace watch_list_app::server {

class HandlerBase {
 private:
  static std::vector<std::reference_wrapper<HandlerBase const>> registered_handlers_;
  struct {
    int call_delete_ = 0;
    int call_get_ = 0;
    int call_patch_ = 0;
    int call_post_ = 0;
    int call_put_ = 0;
    int error_4xx_delete_ = 0;
    int error_4xx_get_ = 0;
    int error_4xx_patch_ = 0;
    int error_4xx_post_ = 0;
    int error_4xx_put_ = 0;
    int error_5xx_delete_ = 0;
    int error_5xx_get_ = 0;
    int error_5xx_patch_ = 0;
    int error_5xx_post_ = 0;
    int error_5xx_put_ = 0;
  } call_metrics_;

  friend class HandlerHealthCheck;

 protected:
  ServerLogger logger_;
  std::string const handler_name_;

 private:
  /// @brief A fallback handler for registered endpoints that don't have a handler.
  ///  This shouldn't be called and indicates a bug if it is.
  HandlerError missing_handler(HTTPMethod method, httplib::Request const& req, httplib::Response& res);

 protected:
  explicit HandlerBase(std::string const& handler_name);

  [[nodiscard]] static auto const& registered_handlers() {
    return registered_handlers_;
  }

  template <typename Derived>
  [[nodiscard]] OptionalServerGenericError register_endpoints_internal(httplib::Server* server) {
    if (HandlerTraits<Derived>::handle_delete) {
      server->Delete(HandlerTraits<Derived>::resource_pattern, [&](httplib::Request const& req, httplib::Response& res) -> void {
        HandlerTimer timer(handler_name_, HTTPMethod::HTTP_DELETE);
        ++call_metrics_.call_delete_;
        if (auto const err = handle_delete(req, res)) {
          if (err->request_info.return_code >= 500) {
            ++call_metrics_.error_5xx_delete_;
          } else {
            ++call_metrics_.error_4xx_delete_;
          }
          logger_.error("Failed to handle [DELETE] request [{}]", format_error(err));
        }
      });
    }

    if (HandlerTraits<Derived>::handle_get) {
      server->Get(HandlerTraits<Derived>::resource_pattern, [&](httplib::Request const& req, httplib::Response& res) -> void {
        HandlerTimer timer(handler_name_, HTTPMethod::HTTP_GET);
        ++call_metrics_.call_get_;
        if (auto const err = handle_get(req, res)) {
          if (err->request_info.return_code >= 500) {
            ++call_metrics_.error_5xx_get_;
          } else {
            ++call_metrics_.error_4xx_get_;
          }
          logger_.error("Failed to handle [GET] request [{}]", format_error(err));
        }
      });
    }

    if (HandlerTraits<Derived>::handle_patch) {
      server->Patch(HandlerTraits<Derived>::resource_pattern, [&](httplib::Request const& req, httplib::Response& res) -> void {
        HandlerTimer timer(handler_name_, HTTPMethod::HTTP_PATCH);
        ++call_metrics_.call_patch_;
        if (auto const err = handle_patch(req, res)) {
          if (err->request_info.return_code >= 500) {
            ++call_metrics_.error_5xx_patch_;
          } else {
            ++call_metrics_.error_4xx_patch_;
          }
          logger_.error("Failed to handle [PATCH] request [{}]", format_error(err));
        }
      });
    }

    if (HandlerTraits<Derived>::handle_post) {
      server->Post(HandlerTraits<Derived>::resource_pattern, [&](httplib::Request const& req, httplib::Response& res) -> void {
        HandlerTimer timer(handler_name_, HTTPMethod::HTTP_POST);
        ++call_metrics_.call_post_;
        if (auto const err = handle_post(req, res)) {
          if (err->request_info.return_code >= 500) {
            ++call_metrics_.error_5xx_post_;
          } else {
            ++call_metrics_.error_4xx_post_;
          }
          logger_.error("Failed to handle [POST] request [{}]", format_error(err));
        }
      });
    }

    if (HandlerTraits<Derived>::handle_put) {
      server->Put(HandlerTraits<Derived>::resource_pattern, [&](httplib::Request const& req, httplib::Response& res) -> void {
        HandlerTimer timer(handler_name_, HTTPMethod::HTTP_PUT);
        ++call_metrics_.call_put_;
        if (auto const err = handle_put(req, res)) {
          if (err->request_info.return_code >= 500) {
            ++call_metrics_.error_5xx_put_;
          } else {
            ++call_metrics_.error_4xx_put_;
          }
          logger_.error("Failed to handle [PUT] request [{}]", format_error(err));
        }
      });
    }

    registered_handlers_.insert(registered_handlers_.cend(), *this);
    return std::nullopt;
  }

  [[nodiscard]] virtual OptionalHandlerError handle_delete(httplib::Request const& req, httplib::Response& res);
  [[nodiscard]] virtual OptionalHandlerError handle_get(httplib::Request const& req, httplib::Response& res);
  [[nodiscard]] virtual OptionalHandlerError handle_patch(httplib::Request const& req, httplib::Response& res);
  [[nodiscard]] virtual OptionalHandlerError handle_post(httplib::Request const& req, httplib::Response& res);
  [[nodiscard]] virtual OptionalHandlerError handle_put(httplib::Request const& req, httplib::Response& res);

  [[nodiscard]] auto const& call_metrics() const {
    return call_metrics_;
  }

 public:
  virtual ~HandlerBase() = default;

  [[nodiscard]] std::string const& handler_name() const {
    return handler_name_;
  }

  [[nodiscard]] virtual OptionalServerGenericError register_endpoints(httplib::Server* server) = 0;
};

template <class T>
struct HandlerInstance {
  static T& instance() {
    static T instance;
    return instance;
  }
};

}  // namespace watch_list_app::server

#endif  // HANDLER_BASE_HPP_
