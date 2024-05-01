/**
 * @file server-listener.hpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#ifndef SERVER_LISTENER_HPP_
#define SERVER_LISTENER_HPP_

#include <pistache/endpoint.h>
#include <pistache/net.h>
#include <pistache/router.h>

#include <memory>

// #include "watch-list-server/http-utils.hpp"
#include "watch-list-server/server-generic-error.hpp"
#include "watch-list-server/server-logger.hpp"

namespace watch_list_app::server {

class ServerListener {
 private:
  ServerLogger logger_;
  Pistache::Address server_address_;
  std::unique_ptr<Pistache::Rest::Router> router_;
  std::unique_ptr<Pistache::Http::Endpoint> server_;

 private:
  [[nodiscard]] OptionalServerGenericError register_routes();

 public:
  ServerListener();

  [[nodiscard]] OptionalServerGenericError initialize();
  [[nodiscard]] OptionalServerGenericError run();
};

}  // namespace watch_list_app::server

#endif  // SERVER_LISTENER_HPP_
