/**
 * @file server-listener.hpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#ifndef SERVER_LISTENER_HPP_
#define SERVER_LISTENER_HPP_

#include <httplib/httplib.h>

#include <memory>

#include "watch-list-server/http-utils.hpp"
#include "watch-list-server/server-generic-error.hpp"
#include "watch-list-server/server-logger.hpp"

namespace watch_list_app::server {

class ServerListener {
 private:
  ServerLogger logger_;
  std::unique_ptr<httplib::Server> server_;

 private:
  [[nodiscard]] OptionalServerGenericError register_routes();

 public:
  ServerListener();

  [[nodiscard]] OptionalServerGenericError initialize();
  [[nodiscard]] OptionalServerGenericError run();
};

}  // namespace watch_list_app::server

#endif  // SERVER_LISTENER_HPP_
