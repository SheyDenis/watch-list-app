/**
 * @file server-listener.cpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#include "watch-list-server/server-listener.hpp"

#include <fmt/core.h>
#include <httplib.h>

#include <cerrno>
#include <cstddef>
#include <functional>
#include <optional>
#include <string>

#include "watch-list-server/dal/database.hpp"
#include "watch-list-server/handlers/handler-base.hpp"
#include "watch-list-server/handlers/handler-exception.hpp"
#include "watch-list-server/handlers/handler-health-check.hpp"
#include "watch-list-server/handlers/handler-index.hpp"
#include "watch-list-server/handlers/handler-users.hpp"
#include "watch-list-server/settings/server-settings-models.hpp"
#include "watch-list-server/settings/server-settings.hpp"

namespace watch_list_app::server {

ServerListener::ServerListener() : logger_("ServerListener") {}

OptionalServerGenericError ServerListener::register_routes() {
  logger_.info("Registering handlers");

  server_->set_exception_handler(handlers::HandlerTraits<handlers::HandlerException>::handle_exception);
  // TODO - Add 404 handler.

  // Admin routes.
  handlers::HandlerBase::register_endpoints<handlers::HandlerUsers>(server_.get(), logger_, HTTPMethod::HTTP_GET, "/admin/users/?");
  handlers::HandlerBase::register_endpoints<handlers::HandlerUsers>(server_.get(), logger_, HTTPMethod::HTTP_GET, "/admin/users/:uid");

  // API routes.
  handlers::HandlerBase::register_endpoints<handlers::HandlerIndex>(server_.get(), logger_, HTTPMethod::HTTP_GET, "/?");
  handlers::HandlerBase::register_endpoints<handlers::HandlerHealthCheck>(server_.get(), logger_, HTTPMethod::HTTP_GET, "/health/?");

  logger_.info("Finished registering handlers");

  return std::nullopt;
}

OptionalServerGenericError ServerListener::initialize() {
  if (auto err = dal::Database::initialize(settings::ServerSettings::database_settings())) {
    return err;
  }

  auto const& httplib_settings = settings::ServerSettings::httplib_settings();
  server_ = std::make_unique<httplib::Server>();

  if (settings::ServerSettings::server_settings().log_requests) {
    server_->set_logger([](httplib::Request const& req, [[maybe_unused]] httplib::Response const& res) {
      ServerLogger request_logger("RequestLogger");
      request_logger.info("Received [{}] request from [{}] to [{}]", req.method, req.remote_addr, req.target);
    });
  }

  server_->set_keep_alive_max_count(httplib_settings.keep_alive_max_count);
  server_->set_keep_alive_timeout(httplib_settings.keep_alive_timeout_sec);

  server_->set_read_timeout(httplib_settings.read_timeout_sec, httplib_settings.read_timeout_usec);
  server_->set_write_timeout(httplib_settings.write_timeout_sec, httplib_settings.write_timeout_usec);
  server_->set_idle_interval(httplib_settings.idle_interval_sec, httplib_settings.idle_interval_usec);
  server_->set_payload_max_length(httplib_settings.payload_max_length);

  auto thread_pool_size = httplib_settings.thread_pool_size;
  server_->new_task_queue = [thread_pool_size]() -> httplib::ThreadPool* { return new httplib::ThreadPool(thread_pool_size); };

  return register_routes();
}

OptionalServerGenericError ServerListener::run() {
  std::string const& server_address(settings::ServerSettings::server_settings().server_address);
  int const server_port = settings::ServerSettings::server_settings().server_port;

  logger_.info("Start listening on [{}:{}]", server_address, server_port);

  if (!server_->listen(server_address, server_port)) {
    return ServerGenericError(fmt::format("Failed to start listening on [{}:{}]", server_address, server_port), std::nullopt, errno);
  }

  return std::nullopt;
}

}  // namespace watch_list_app::server
