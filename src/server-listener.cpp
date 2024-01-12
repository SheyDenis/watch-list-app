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
#include <functional>
#include <optional>
#include <string>

#include "watch-list-server/dev-utils.hpp"
#include "watch-list-server/handlers/handler-base.hpp"
#include "watch-list-server/handlers/handler-health-check.hpp"
#include "watch-list-server/handlers/handler-index.hpp"
#include "watch-list-server/settings/server-settings.hpp"

namespace watch_list_app::server {

ServerListener::ServerListener() : logger_("ServerListener") {}

OptionalServerGenericError ServerListener::initialize() {
  LOG_NOT_IMPLEMENTED();

  server_ = std::make_unique<httplib::Server>();

  //   server_.set_logger([]( auto const& req, auto  const& res) {
  //  your_logger(req, res);
  //});

  //   svr.set_error_handler([](const auto& req, auto& res) {
  //  auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
  //  char buf[BUFSIZ];
  //  snprintf(buf, sizeof(buf), fmt, res.status);
  //  res.set_content(buf, "text/html");
  //});

  //   svr.set_exception_handler([](const auto& req, auto& res, std::exception_ptr ep) {
  //  auto fmt = "<h1>Error 500</h1><p>%s</p>";
  //  char buf[BUFSIZ];
  //  try {
  //    std::rethrow_exception(ep);
  //  } catch (std::exception &e) {
  //    snprintf(buf, sizeof(buf), fmt, e.what());
  //  } catch (...) { // See the following NOTE
  //    snprintf(buf, sizeof(buf), fmt, "Unknown Exception");
  //  }
  //  res.set_content(buf, "text/html");
  //  res.status = StatusCode::InternalServerError_500;
  //});

  // server_->set_keep_alive_max_count(0);
  // server_->set_keep_alive_timeout(0);
  //
  // server_->set_read_timeout(5, 0);
  // server_->set_write_timeout(5, 0);
  // server_->set_idle_interval(0, 100000); // 100 milliseconds
  //
  // server_->new_task_queue = []()->httplib::ThreadPool* { return new httplib::ThreadPool(5); };  // TODO - Get from settings.

  std::vector<std::reference_wrapper<HandlerBase>> handlers{
      HandlerInstance<HandlerHealthCheck>::instance(),
      HandlerInstance<HandlerIndex>::instance(),
  };
  logger_.info("Registering [{}] handlers", handlers.size());

  for (auto const& itr : handlers) {
    logger_.info("Registering handler [{}]", itr.get().handler_name());
    if (auto err = itr.get().register_endpoints(server_.get())) {
      logger_.error("Failed to register handler [{}]", itr.get().handler_name());
      return err;
    }
  }

  logger_.info("Finished registering handlers");

  return std::nullopt;
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
