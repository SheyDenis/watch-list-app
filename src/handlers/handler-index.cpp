/**
 * @file handler-index.cpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#include "watch-list-server/handlers/handler-index.hpp"

#include <memory>
#include <optional>

namespace watch_list_app::server::handlers {

HandlerIndex::HandlerIndex() : HandlerBase("HandlerIndex") {}

void HandlerIndex::handle_get(RestRequest const& request, HttpResponseWriter response) {
  response.send(HttpCode::Ok, "Potato!", MimeMediaType(MimeType::Text, MimeSubtype::Plain));
}

// OptionalHandlerError HandlerIndex::handle_get_impl([[maybe_unused]] httplib::Request const& req, httplib::Response& res) {
//   res.set_content("Potato!", "text/plain");
//   res.status = httplib::StatusCode::OK_200;
//   return std::nullopt;
// }

}  // namespace watch_list_app::server::handlers
