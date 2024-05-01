/**
 * @file server-listener.cpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#include "watch-list-server/server-listener.hpp"

#include <fmt/core.h>
#include <pistache/net.h>

#include <cerrno>
#include <cstddef>
#include <functional>
#include <optional>
#include <string>

// #include "watch-list-server/dal/database-builder.hpp"
#include "watch-list-server/handlers/handler-base.hpp"
// #include "watch-list-server/handlers/handler-exception.hpp"
// #include "watch-list-server/handlers/handler-health-check.hpp"
#include "watch-list-server/handlers/handler-index.hpp"
// #include "watch-list-server/handlers/handler-request-error.hpp"
// #include "watch-list-server/handlers/handler-users.hpp"
#include "watch-list-server/middleware/request-logger-middleware.hpp"
#include "watch-list-server/pistache_utils.hpp"
#include "watch-list-server/settings/server-settings-models.hpp"
#include "watch-list-server/settings/server-settings.hpp"

namespace watch_list_app::server {

ServerListener::ServerListener() : logger_("ServerListener"), server_address_(), router_(nullptr), server_(nullptr) {}

OptionalServerGenericError ServerListener::register_routes() {
  logger_.info("Registering handlers");

  //  server_->set_exception_handler(handlers::HandlerTraits<handlers::HandlerException>::handle_exception);
  //  server_->set_error_handler(handlers::HandlerTraits<handlers::HandlerRequestError>::handle_error);

  // Admin routes.
  //  auto users_handler = handlers::HandlerInstance<handlers::HandlerUsers>::instance();
  //  router_->addRoute(Pistache::Http::Method::Get, "/admin/users",
  //  Pistache::Rest::Routes::bind(&handlers::HandlerUsers::handle_list_users, &users_handler));
  //  router_->addRoute(Pistache::Http::Method::Get, "/admin/users/:d",
  //  Pistache::Rest::Routes::bind(&handlers::HandlerUsers::handle_get_user, &users_handler));

  // API routes.
  auto index_handler = handlers::HandlerInstance<handlers::HandlerIndex>::instance();
  router_->addRoute(HttpMethod::Get, "/", Pistache::Rest::Routes::bind(&handlers::HandlerIndex::handle_get, index_handler));
  //  if (auto err = handlers::HandlerBase::register_endpoints<handlers::HandlerIndex>(server_.get(), logger_, HTTPMethod::HTTP_GET, "/?"))
  //  {
  //    return err;
  //  }
  //  if (auto err = handlers::HandlerBase::register_endpoints<handlers::HandlerHealthCheck>(
  //          server_.get(), logger_, HTTPMethod::HTTP_GET, "/health/?")) {
  //    return err;
  //  }

  logger_.info("Finished registering handlers");

  return std::nullopt;
}

OptionalServerGenericError ServerListener::initialize() {
  //  if (auto err = dal::DatabaseBuilder::initialize(settings::ServerSettings::database_settings())) {
  //    return err;
  //  }

  auto const& pistache_settings = settings::ServerSettings::pistache_settings();
  auto pistache_options = Pistache::Http::Endpoint::options();
  pistache_options.backlog(pistache_settings.max_backlog);
  pistache_options.maxRequestSize(pistache_settings.max_request_size);
  pistache_options.maxResponseSize(pistache_settings.max_response_size);
  pistache_options.headerTimeout(pistache_settings.header_timeout);
  pistache_options.keepaliveTimeout(pistache_settings.keep_alive_timeout);
  pistache_options.sslHandshakeTimeout(pistache_settings.ssl_handshake_timeout);
  pistache_options.logger(std::make_shared<PistacheServerLogger>("PistacheServerLogger"));

  router_ = std::make_unique<Pistache::Rest::Router>();
  server_address_ = Pistache::Address(settings::ServerSettings::server_settings().server_address,
                                      settings::ServerSettings::server_settings().server_port);
  server_ = std::make_unique<Pistache::Http::Endpoint>(server_address_);
  if (settings::ServerSettings::server_settings().log_requests) {
    auto request_logger_middleware = handlers::HandlerInstance<middleware::RequestLoggerMiddleware>::instance();
    request_logger_middleware->enable();
    router_->addMiddleware(
        Pistache::Rest::Routes::middleware(&middleware::RequestLoggerMiddleware::log_request, request_logger_middleware));
  }

  server_->init(pistache_options);

  if (auto err = register_routes()) {
    return err;
  }
  server_->setHandler(router_->handler());
  return std::nullopt;
}

OptionalServerGenericError ServerListener::run() {
  logger_.info("Start listening on [{}:{}]", server_address_.host(), server_address_.port().toString());

  server_->serve();

  return std::nullopt;
}

}  // namespace watch_list_app::server
