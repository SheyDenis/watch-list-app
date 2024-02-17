/**
 * @file handler-users.cpp
 * @author denis
 * @brief
 * @date 12024-01-18
 *
 */

#include "watch-list-server/handlers/handler-users.hpp"

#include <httplib/httplib.h>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>

#include <memory>
#include <optional>

#include "watch-list-server/models/base-model.hpp"
#include "watch-list-server/models/user-model.hpp"

namespace watch_list_app::server::handlers {

HandlerUsers::HandlerUsers() : HandlerBase("HandlerUsers") {}

// OptionalServerGenericError HandlerUsers::register_endpoints(httplib::Server* server) {
//   return HandlerBase::register_endpoints_internal<HandlerUsers>(server);
// }

OptionalHandlerError HandlerUsers::handle_get_impl(httplib::Request const& req, httplib::Response& res) {
  if (req.path_params.find("uid") != req.path_params.cend()) {
    return handle_get_user(req, res);
  }
  return handle_list_users(req, res);
}

OptionalHandlerError HandlerUsers::handle_get_user(httplib::Request const& req, httplib::Response& res) {
  std::string const uuid(req.path_params.at("uid"));

  bool found;
  models::UserModel model;
  models::UserModel::find(uuid, model, found);
  rapidjson::Document data;
  if (!found) {
    res.status = httplib::StatusCode::NotFound_404;
  } else {
    model.serialize(data, data.GetAllocator());
    res.set_content(json::JSONUtils::dump(data, 2), "application/json");
    res.status = httplib::StatusCode::OK_200;
  }
  return std::nullopt;
}

OptionalHandlerError HandlerUsers::handle_list_users([[maybe_unused]] httplib::Request const& req, httplib::Response& res) {
  rapidjson::Document data(rapidjson::kArrayType);

  std::vector<models::UserModel> models;
  models::UserModel::scan(models);

  for (auto const& itr : models) {
    rapidjson::Value v;
    itr.serialize(v, data.GetAllocator());
    data.PushBack(v, data.GetAllocator());
  }

  res.set_content(json::JSONUtils::dump(data, 2), "application/json");
  res.status = httplib::StatusCode::OK_200;
  return std::nullopt;
}

}  // namespace watch_list_app::server::handlers
