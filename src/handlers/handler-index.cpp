/**
 * @file handler-index.cpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#include "watch-list-server/handlers/handler-index.hpp"

#include <httplib/httplib.h>

#include <memory>
#include <optional>

namespace watch_list_app::server {

HandlerIndex::HandlerIndex() : HandlerBase("HandlerIndex") {}

OptionalServerGenericError HandlerIndex::register_endpoints(httplib::Server* server) {
  return HandlerBase::register_endpoints_internal<HandlerIndex>(server);
}

OptionalHandlerError HandlerIndex::handle_get(httplib::Request const& req, httplib::Response& res) {
  res.set_content("Potato!", "text/plain");
  res.status = httplib::StatusCode::OK_200;
  return std::nullopt;
}

}  // namespace watch_list_app::server
