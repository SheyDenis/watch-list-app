/**
 * @file handler-exception.hpp
 * @author denis
 * @brief
 * @date 12024-01-13
 *
 */

#ifndef HANDLER_EXCEPTION_HPP_
#define HANDLER_EXCEPTION_HPP_

#include <httplib/httplib.h>
#include <rapidjson/allocators.h>
#include <rapidjson/document.h>
#include <rapidjson/encodings.h>
#include <rapidjson/rapidjson.h>

#include <stdexcept>
#include <string>

#include "watch-list-server/handlers/handler-base.hpp"
#include "watch-list-server/handlers/handler-error.hpp"
#include "watch-list-server/handlers/handler-traits.hpp"
#include "watch-list-server/json/json-utils.hpp"
#include "watch-list-server/server-generic-error.hpp"

namespace watch_list_app::server::handlers {

class HandlerException : public HandlerBase {
 private:
  friend struct HandlerInstance<HandlerException>;

 protected:
  HandlerException() : HandlerBase("HandlerException") {}

 public:
  ~HandlerException() override = default;

  [[nodiscard]] OptionalHandlerError handle_exception(httplib::Request const& req, httplib::Response& res, std::exception_ptr const& exp) {
    rapidjson::Document res_json(rapidjson::Type::kObjectType);

    res.status = httplib::StatusCode::InternalServerError_500;
    res_json.AddMember("status", res.status, res_json.GetAllocator());
    res_json.AddMember("error", "Something went wrong, sorry.", res_json.GetAllocator());

    res.set_content(json::JSONUtils::dump(res_json), "application/json");

    std::string ex_str;
    try {
      std::rethrow_exception(exp);
    } catch (std::exception const& ex) {
      ex_str.assign(ex.what());
    } catch (...) {
      ex_str.assign("Unknown Exception");
    }

    logger_.error("Exception thrown while handling [{} {}] [ex={}]", req.method, req.target, ex_str);
    return std::nullopt;
  }
};

template <>
struct HandlerTraits<HandlerException> {
  using HandlerType = HandlerException;

  static void handle_exception(httplib::Request const& req, httplib::Response& res, std::exception_ptr const& exp) {
    handlers::HandlerInstance<HandlerType>::instance().handle_exception(req, res, exp);
  }
};

}  // namespace watch_list_app::server::handlers

#endif  // HANDLER_EXCEPTION_HPP_
