/**
 * @file handler-health-check.cpp
 * @author denis
 * @brief
 * @date 12024-01-12
 *
 */

#include "watch-list-server/handlers/handler-health-check.hpp"

#include <bits/chrono.h>
#include <httplib/httplib.h>
#include <rapidjson/allocators.h>
#include <rapidjson/document.h>
#include <rapidjson/encodings.h>
#include <rapidjson/rapidjson.h>

#include <functional>
#include <optional>
#include <vector>

#include "watch-list-server/json/json-utils.hpp"

namespace watch_list_app::server {

HandlerHealthCheck::HandlerHealthCheck() : HandlerBase("HandlerHealthCheck") {}

OptionalServerGenericError HandlerHealthCheck::register_endpoints(httplib::Server* server) {
  return HandlerBase::register_endpoints_internal<HandlerHealthCheck>(server);
}

OptionalHandlerError HandlerHealthCheck::handle_get(httplib::Request const& req, httplib::Response& res) {
  rapidjson::Document res_json(rapidjson::Type::kObjectType);

  res_json.AddMember("timestamp",
                     std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(),
                     res_json.GetAllocator());

  for (auto const& handler : registered_handlers()) {
    auto const& handler_metrics = handler.get().call_metrics();
    rapidjson::Value handler_json(rapidjson::Type::kObjectType);

    rapidjson::Value calls_json(rapidjson::Type::kObjectType);
    calls_json.AddMember("delete", handler_metrics.call_delete_, res_json.GetAllocator());
    calls_json.AddMember("get", handler_metrics.call_get_, res_json.GetAllocator());
    calls_json.AddMember("patch", handler_metrics.call_patch_, res_json.GetAllocator());
    calls_json.AddMember("post", handler_metrics.call_post_, res_json.GetAllocator());
    calls_json.AddMember("put", handler_metrics.call_put_, res_json.GetAllocator());

    rapidjson::Value errors_json(rapidjson::Type::kObjectType);

    rapidjson::Value errors_400_json(rapidjson::Type::kObjectType);
    errors_400_json.AddMember("delete", handler_metrics.error_4xx_delete_, res_json.GetAllocator());
    errors_400_json.AddMember("get", handler_metrics.error_4xx_get_, res_json.GetAllocator());
    errors_400_json.AddMember("patch", handler_metrics.error_4xx_patch_, res_json.GetAllocator());
    errors_400_json.AddMember("post", handler_metrics.error_4xx_post_, res_json.GetAllocator());
    errors_400_json.AddMember("put", handler_metrics.error_4xx_put_, res_json.GetAllocator());

    rapidjson::Value errors_500_json(rapidjson::Type::kObjectType);
    errors_500_json.AddMember("delete", handler_metrics.error_5xx_delete_, res_json.GetAllocator());
    errors_500_json.AddMember("get", handler_metrics.error_5xx_get_, res_json.GetAllocator());
    errors_500_json.AddMember("patch", handler_metrics.error_5xx_patch_, res_json.GetAllocator());
    errors_500_json.AddMember("post", handler_metrics.error_5xx_post_, res_json.GetAllocator());
    errors_500_json.AddMember("put", handler_metrics.error_5xx_put_, res_json.GetAllocator());

    handler_json.AddMember("calls", calls_json, res_json.GetAllocator());

    errors_json.AddMember("4xx", errors_400_json, res_json.GetAllocator());
    errors_json.AddMember("5xx", errors_500_json, res_json.GetAllocator());
    handler_json.AddMember("errors", errors_json, res_json.GetAllocator());

    res_json.AddMember(rapidjson::StringRef(handler.get().handler_name().c_str()), handler_json, res_json.GetAllocator());
  }

  res.set_content(json::JSONUtils::dump(res_json, 2), "application/json");
  res.status = httplib::StatusCode::OK_200;
  return std::nullopt;
}

}  // namespace watch_list_app::server
