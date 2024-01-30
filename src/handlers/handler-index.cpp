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

namespace watch_list_app::server::handlers {

HandlerIndex::HandlerIndex() : HandlerBase("HandlerIndex") {}

OptionalHandlerError HandlerIndex::handle_get_impl(httplib::Request const& req, httplib::Response& res) {
  res.set_content("Potato!", "text/plain");
  res.status = httplib::StatusCode::OK_200;
  return std::nullopt;
}

}  // namespace watch_list_app::server::handlers
