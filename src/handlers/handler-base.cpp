/**
 * @file handler-base.cpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#include "watch-list-server/handlers/handler-base.hpp"

#include <stdexcept>

// #include "watch-list-server/handlers/handler-timer.hpp"
#include "watch-list-server/server-error.hpp"

namespace watch_list_app::server::handlers {

std::vector<std::reference_wrapper<HandlerBase const>> HandlerBase::registered_handlers_;

HandlerBase::HandlerBase(std::string handler_name) : call_metrics_({}), logger_(handler_name), handler_name_(handler_name) {}

// HandlerError HandlerBase::missing_handler([[maybe_unused]] HTTPMethod method,
//                                           [[maybe_unused]] httplib::Request const& req,
//                                           [[maybe_unused]] httplib::Response& res) {
//   throw std::runtime_error("Handler was registered and called but not implemented");
// }
//
// OptionalHandlerError HandlerBase::handle_delete_impl(httplib::Request const& req, httplib::Response& res) {
//   return missing_handler(HTTPMethod::HTTP_DELETE, req, res);
// }
//
// OptionalHandlerError HandlerBase::handle_get_impl(httplib::Request const& req, httplib::Response& res) {
//   return missing_handler(HTTPMethod::HTTP_GET, req, res);
// }
//
// OptionalHandlerError HandlerBase::handle_patch_impl(httplib::Request const& req, httplib::Response& res) {
//   return missing_handler(HTTPMethod::HTTP_PATCH, req, res);
// }
//
// OptionalHandlerError HandlerBase::handle_post_impl(httplib::Request const& req, httplib::Response& res) {
//   return missing_handler(HTTPMethod::HTTP_POST, req, res);
// }
//
// OptionalHandlerError HandlerBase::handle_put_impl(httplib::Request const& req, httplib::Response& res) {
//   return missing_handler(HTTPMethod::HTTP_PUT, req, res);
// }
//
// OptionalHandlerError HandlerBase::handle_delete(httplib::Request const& req, httplib::Response& res) {
//   return handle_delete_impl(req, res);
// }
//
// OptionalHandlerError HandlerBase::handle_get(httplib::Request const& req, httplib::Response& res) {
//   handlers::HandlerTimer timer(handler_name(), HTTPMethod::HTTP_GET);
//   ++call_metrics_.call_get_;
//   if (auto const err = handle_get_impl(req, res)) {
//     if (err->request_info.return_code >= 500) {  // FIXME - Replace magic numbers.
//       ++call_metrics_.error_5xx_get_;
//     } else {
//       ++call_metrics_.error_4xx_get_;
//     }
//     logger_.error("Failed to handle [GET] request [{}]", format_error(err));
//     return err;
//   }
//   return std::nullopt;
// }
//
// OptionalHandlerError HandlerBase::handle_patch(httplib::Request const& req, httplib::Response& res) {
//   return handle_patch_impl(req, res);
// }
//
// OptionalHandlerError HandlerBase::handle_post(httplib::Request const& req, httplib::Response& res) {
//   return handle_post_impl(req, res);
// }
//
// OptionalHandlerError HandlerBase::handle_put(httplib::Request const& req, httplib::Response& res) {
//   return handle_put_impl(req, res);
// }

}  // namespace watch_list_app::server::handlers
