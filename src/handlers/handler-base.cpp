/**
 * @file handler-base.cpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#include "watch-list-server/handlers/handler-base.hpp"

#include <httplib/httplib.h>

namespace watch_list_app::server {

std::vector<std::reference_wrapper<HandlerBase const>> HandlerBase::registered_handlers_;

HandlerBase::HandlerBase(std::string const& handler_name) : logger_(handler_name), handler_name_(handler_name) {}

HandlerError HandlerBase::missing_handler(HTTPMethod method, [[maybe_unused]] httplib::Request const& req, httplib::Response& res) {
  res.set_content("Something went wrong!", "text/plain");
  res.status = httplib::StatusCode::InternalServerError_500;
  return {method, handler_name_, {500, req.params}, "Handler was registered and called but not implemented"};
}

OptionalHandlerError HandlerBase::handle_delete(httplib::Request const& req, httplib::Response& res) {
  return missing_handler(HTTPMethod::HTTP_DELETE, req, res);
}

OptionalHandlerError HandlerBase::handle_get(httplib::Request const& req, httplib::Response& res) {
  return missing_handler(HTTPMethod::HTTP_GET, req, res);
}

OptionalHandlerError HandlerBase::handle_patch(httplib::Request const& req, httplib::Response& res) {
  return missing_handler(HTTPMethod::HTTP_PATCH, req, res);
}

OptionalHandlerError HandlerBase::handle_post(httplib::Request const& req, httplib::Response& res) {
  return missing_handler(HTTPMethod::HTTP_POST, req, res);
}

OptionalHandlerError HandlerBase::handle_put(httplib::Request const& req, httplib::Response& res) {
  return missing_handler(HTTPMethod::HTTP_PUT, req, res);
}

}  // namespace watch_list_app::server
