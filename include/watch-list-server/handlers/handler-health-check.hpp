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

#include "watch-list-server/handlers/handler-base.hpp"
#include "watch-list-server/handlers/handler-error.hpp"
#include "watch-list-server/handlers/handler-traits.hpp"

namespace watch_list_app::server::handlers {

class HandlerHealthCheck : public HandlerBase {
 private:
  friend struct HandlerInstance<HandlerHealthCheck>;

 protected:
  HandlerHealthCheck();

 public:
  ~HandlerHealthCheck() override = default;

  [[nodiscard]] OptionalHandlerError handle_get_impl(httplib::Request const& req, httplib::Response& res) override;
};

}  // namespace watch_list_app::server::handlers

#endif  // HANDLER_HEALTH_CHECK_HPP_
