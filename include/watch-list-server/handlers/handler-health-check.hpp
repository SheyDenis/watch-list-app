/**
 * @file handler-health-check.hpp
 * @author denis
 * @brief
 * @date 12024-01-12
 *
 */

#ifndef HANDLER_HEALTH_CHECK_HPP_
#define HANDLER_HEALTH_CHECK_HPP_

#include <httplib/httplib.h>

#include <optional>

#include "watch-list-server/handlers/handler-base.hpp"
#include "watch-list-server/handlers/handler-error.hpp"
#include "watch-list-server/handlers/handler-traits.hpp"
#include "watch-list-server/server-generic-error.hpp"

namespace watch_list_app::server {

class HandlerHealthCheck : public HandlerBase {
 private:
  friend struct HandlerInstance<HandlerHealthCheck>;

 protected:
  HandlerHealthCheck();

  [[nodiscard]] OptionalHandlerError handle_get(httplib::Request const& req, httplib::Response& res) override;

 public:
  ~HandlerHealthCheck() override = default;

  [[nodiscard]] OptionalServerGenericError register_endpoints(httplib::Server* server) override;
};

template <>
struct HandlerTypes<HandlerHealthCheck> {
  static constexpr char const* resource_pattern = "/health";

  static bool constexpr handle_delete = false;
  static bool constexpr handle_get = true;
  static bool constexpr handle_patch = false;
  static bool constexpr handle_post = false;
  static bool constexpr handle_put = false;
};

}  // namespace watch_list_app::server

#endif  // HANDLER_HEALTH_CHECK_HPP_
