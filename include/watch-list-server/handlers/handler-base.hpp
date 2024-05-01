/**
 * @file handler-base.hpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#ifndef HANDLER_BASE_HPP_
#define HANDLER_BASE_HPP_

#include <fmt/core.h>
#include <pistache/router.h>

#include <algorithm>
#include <functional>
#include <optional>
#include <string>
#include <vector>

// #include "watch-list-server/handlers/handler-error.hpp"
// #include "watch-list-server/handlers/handler-traits.hpp"
// #include "watch-list-server/http-utils.hpp"
#include "watch-list-server/pistache_utils.hpp"
#include "watch-list-server/server-generic-error.hpp"
#include "watch-list-server/server-logger.hpp"

namespace watch_list_app::server::handlers {

template <class T>
struct HandlerInstance {
  static std::shared_ptr<T> instance() {
    static std::shared_ptr<T> handler_instance(new T());
    return handler_instance;
  }
};

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
  //  /// @brief A fallback handler for registered endpoints that don't have a handler.
  //  ///  This shouldn't be called and indicates a bug if it is.
  //  HandlerError missing_handler(HTTPMethod method, httplib::Request const& req, httplib::Response& res);

 protected:
  explicit HandlerBase(std::string handler_name);

  [[nodiscard]] static auto const& registered_handlers() {
    return registered_handlers_;
  }

  //  [[nodiscard]] virtual OptionalHandlerError handle_delete_impl(httplib::Request const& req, httplib::Response& res);
  //  [[nodiscard]] virtual OptionalHandlerError handle_get_impl(httplib::Request const& req, httplib::Response& res);
  //  [[nodiscard]] virtual OptionalHandlerError handle_patch_impl(httplib::Request const& req, httplib::Response& res);
  //  [[nodiscard]] virtual OptionalHandlerError handle_post_impl(httplib::Request const& req, httplib::Response& res);
  //  [[nodiscard]] virtual OptionalHandlerError handle_put_impl(httplib::Request const& req, httplib::Response& res);

  [[nodiscard]] auto const& call_metrics() const {
    return call_metrics_;
  }

 public:
  //  template <typename T>
  //  [[nodiscard]] static OptionalServerGenericError register_endpoints(httplib::Server* server,
  //                                                                     ServerLogger& logger,
  //                                                                     HTTPMethod method,
  //                                                                     std::string const& pattern) {
  //    auto const& handler_instance = HandlerInstance<T>::instance();
  //    logger.info("Registering handler [{} {}] to [{}]", http_method_to_string(method), handler_instance.handler_name(), pattern);
  //
  //    switch (method) {
  //      case HTTPMethod::HTTP_DELETE:
  //        server->Delete(pattern, handlers::HandlerTraits<T>::handle_get);
  //        break;
  //      case HTTPMethod::HTTP_GET:
  //        server->Get(pattern, handlers::HandlerTraits<T>::handle_get);
  //        break;
  //      case HTTPMethod::HTTP_PATCH:
  //        server->Patch(pattern, handlers::HandlerTraits<T>::handle_get);
  //        break;
  //      case HTTPMethod::HTTP_POST:
  //        server->Post(pattern, handlers::HandlerTraits<T>::handle_get);
  //        break;
  //      case HTTPMethod::HTTP_PUT:
  //        server->Put(pattern, handlers::HandlerTraits<T>::handle_get);
  //        break;
  //      default:
  //        return ServerGenericError(fmt::format("Unable to register handler for unsupported method [{}]", http_method_to_string(method)));
  //    }
  //
  //    auto const& hndlr_name = handler_instance.handler_name();
  //    if (!std::any_of(registered_handlers_.cbegin(),
  //                     registered_handlers_.cend(),
  //                     [&hndlr_name](decltype(registered_handlers_)::value_type const& itr) -> bool {
  //                       return itr.get().handler_name() == hndlr_name;
  //                     })) {
  //      registered_handlers_.insert(registered_handlers_.cend(), HandlerInstance<T>::instance());
  //    }
  //    return std::nullopt;
  //  }

  virtual ~HandlerBase() = default;

  [[nodiscard]] std::string const& handler_name() const {
    return handler_name_;
  }

  //  [[nodiscard]] OptionalHandlerError handle_delete(httplib::Request const& req, httplib::Response& res);
  //  [[nodiscard]] OptionalHandlerError handle_get(httplib::Request const& req, httplib::Response& res);
  //  [[nodiscard]] OptionalHandlerError handle_patch(httplib::Request const& req, httplib::Response& res);
  //  [[nodiscard]] OptionalHandlerError handle_post(httplib::Request const& req, httplib::Response& res);
  //  [[nodiscard]] OptionalHandlerError handle_put(httplib::Request const& req, httplib::Response& res);
};

}  // namespace watch_list_app::server::handlers

#endif  // HANDLER_BASE_HPP_
