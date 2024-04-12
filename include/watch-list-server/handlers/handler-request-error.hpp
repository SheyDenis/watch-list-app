/**
 * @file handler-request-error.hpp
 * @author denis
 * @brief
 * @date 12024-02-09
 *
 */

#ifndef HANDLER_REQUEST_ERROR_HPP_
#define HANDLER_REQUEST_ERROR_HPP_

#include <fmt/core.h>
#include <httplib/httplib.h>
#include <rapidjson/allocators.h>
#include <rapidjson/document.h>
#include <rapidjson/encodings.h>
#include <rapidjson/rapidjson.h>

#include <string>

#include "watch-list-server/handlers/handler-base.hpp"
#include "watch-list-server/handlers/handler-error.hpp"
#include "watch-list-server/handlers/handler-traits.hpp"
#include "watch-list-server/json/json-utils.hpp"

namespace watch_list_app::server::handlers {

class HandlerRequestError : public HandlerBase {
 private:
  friend struct HandlerInstance<HandlerRequestError>;

 protected:
  HandlerRequestError() : HandlerBase("HandlerRequestError") {}

 public:
  ~HandlerRequestError() override = default;

  void handle_error(httplib::Request const& req, httplib::Response& res) {
    if (res.status != httplib::StatusCode::NotFound_404) {
      return;
    }

    if (res.body.empty()) {
      rapidjson::Document res_json(rapidjson::Type::kObjectType);
      res_json.AddMember("status", res.status, res_json.GetAllocator());
      res_json.AddMember("error",
                         rapidjson::Value(fmt::format("Resource [{}] not found", req.path).c_str(), res_json.GetAllocator()).Move(),
                         res_json.GetAllocator());

      res.set_content(json::JSONUtils::dump(res_json), "application/json");
    }
  }
};

template <>
struct HandlerTraits<HandlerRequestError> {
  using HandlerType = HandlerRequestError;

  static void handle_error(httplib::Request const& req, httplib::Response& res) {
    handlers::HandlerInstance<HandlerType>::instance().handle_error(req, res);
  }
};

}  // namespace watch_list_app::server::handlers

#endif  // HANDLER_REQUEST_ERROR_HPP_
