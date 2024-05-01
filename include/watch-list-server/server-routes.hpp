/**
 * @file server-routes.hpp
 * @author denis
 * @brief
 * @date 12024-02-12
 *
 */

#ifndef SERVER_ROUTES_HPP_
#define SERVER_ROUTES_HPP_

#include <fmt/core.h>

#include <initializer_list>
#include <string>

namespace watch_list_app::server {

class ServerRoutes {
 public:
  static auto constexpr kAdminRoute = "admin";

 public:
  ServerRoutes() = delete;
  // TODO - Implement.

  static std::string create_route(std::initializer_list<std::string> route_parts) {
    if ((route_parts.end() - 1)->rfind('/') == (route_parts.end() - 1)->length()) {
    }
  }
};

}  // namespace watch_list_app::server

#endif  // SERVER_ROUTES_HPP_
