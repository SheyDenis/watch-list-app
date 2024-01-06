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

#include <set>
#include <string>

#include "watch-list-server/handlers/handler-error.hpp"
#include "watch-list-server/handlers/handler-traits.hpp"
#include "watch-list-server/http-utils.hpp"
#include "watch-list-server/server-generic-error.hpp"
#include "watch-list-server/server-logger.hpp"

namespace watch_list_app::server {

class HandlerBase {
 protected:
  ServerLogger logger_;

 private:
  HandlerError missing_handler(HTTPMethod method, httplib::Request const& req, httplib::Response& res);

 protected:
  explicit HandlerBase(std::string handler_name);

  template <typename Derived>
  [[nodiscard]] OptionalServerGenericError register_endpoints_internal(httplib::Server* server) {
    if (HandlerTraits<Derived>::handle_delete) {
      server->Delete(HandlerTraits<Derived>::resource_pattern, [&](httplib::Request const& req, httplib::Response& res) -> void {
        if (auto const err = handle_delete(req, res)) {
          logger_.error("Failed to handle [DELETE] request [{}]", *err);
        }
      });
    }

    if (HandlerTraits<Derived>::handle_get) {
      server->Get(HandlerTraits<Derived>::resource_pattern, [&](httplib::Request const& req, httplib::Response& res) -> void {
        if (auto const err = handle_get(req, res)) {
          logger_.error("Failed to handle [GET] request [{}]", *err);
        }
      });
    }

    if (HandlerTraits<Derived>::handle_patch) {
      server->Patch(HandlerTraits<Derived>::resource_pattern, [&](httplib::Request const& req, httplib::Response& res) -> void {
        if (auto const err = handle_patch(req, res)) {
          logger_.error("Failed to handle [PATCH] request [{}]", *err);
        }
      });
    }

    if (HandlerTraits<Derived>::handle_post) {
      server->Post(HandlerTraits<Derived>::resource_pattern, [&](httplib::Request const& req, httplib::Response& res) -> void {
        if (auto const err = handle_post(req, res)) {
          logger_.error("Failed to handle [POST] request [{}]", *err);
        }
      });
    }

    if (HandlerTraits<Derived>::handle_put) {
      server->Put(HandlerTraits<Derived>::resource_pattern, [&](httplib::Request const& req, httplib::Response& res) -> void {
        if (auto const err = handle_put(req, res)) {
          logger_.error("Failed to handle [PUT] request [{}]", *err);
        }
      });
    }

    return std::nullopt;
  }

  [[nodiscard]] virtual OptionalHandlerError handle_delete(httplib::Request const& req, httplib::Response& res);
  [[nodiscard]] virtual OptionalHandlerError handle_get(httplib::Request const& req, httplib::Response& res);
  [[nodiscard]] virtual OptionalHandlerError handle_patch(httplib::Request const& req, httplib::Response& res);
  [[nodiscard]] virtual OptionalHandlerError handle_post(httplib::Request const& req, httplib::Response& res);
  [[nodiscard]] virtual OptionalHandlerError handle_put(httplib::Request const& req, httplib::Response& res);

 public:
  virtual ~HandlerBase() = default;

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
