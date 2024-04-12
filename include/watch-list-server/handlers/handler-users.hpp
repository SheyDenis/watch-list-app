/**
 * @file handler-users.hpp
 * @author denis
 * @brief
 * @date 12024-01-18
 *
 */

#ifndef HANDLER_USERS_HPP_
#define HANDLER_USERS_HPP_

#include <httplib/httplib.h>

#include "watch-list-server/handlers/handler-base.hpp"
#include "watch-list-server/handlers/handler-error.hpp"
#include "watch-list-server/handlers/handler-traits.hpp"

namespace watch_list_app::server::handlers {

class HandlerUsers : public HandlerBase {
 private:
  friend struct HandlerInstance<HandlerUsers>;

 private:
  [[nodiscard]] OptionalHandlerError handle_get_user(httplib::Request const& req, httplib::Response& res);
  [[nodiscard]] OptionalHandlerError handle_list_users(httplib::Request const& req, httplib::Response& res);

 protected:
  HandlerUsers();

 public:
  ~HandlerUsers() override = default;

  [[nodiscard]] OptionalHandlerError handle_get_impl(httplib::Request const& req, httplib::Response& res) override;
};

}  // namespace watch_list_app::server::handlers

#endif  // HANDLER_USERS_HPP_
