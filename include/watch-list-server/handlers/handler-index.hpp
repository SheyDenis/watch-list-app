/**
 * @file handler-index.hpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#ifndef HANDLER_INDEX_HPP_
#define HANDLER_INDEX_HPP_

#include <httplib/httplib.h>

#include "watch-list-server/handlers/handler-base.hpp"
#include "watch-list-server/handlers/handler-error.hpp"
#include "watch-list-server/handlers/handler-traits.hpp"
#include "watch-list-server/server-generic-error.hpp"

namespace watch_list_app::server {

class HandlerIndex : public HandlerBase {
 private:
  friend struct HandlerInstance<HandlerIndex>;

 protected:
 protected:
  HandlerIndex();

  [[nodiscard]] OptionalHandlerError handle_get(httplib::Request const& req, httplib::Response& res) override;

 public:
  ~HandlerIndex() override = default;

  [[nodiscard]] OptionalServerGenericError register_endpoints(httplib::Server* server) override;
};

template <>
struct HandlerTypes<HandlerIndex> {
  static constexpr char const* resource_pattern = "/";

  static bool constexpr handle_delete = false;
  static bool constexpr handle_get = true;
  static bool constexpr handle_patch = false;
  static bool constexpr handle_post = false;
  static bool constexpr handle_put = false;
};

}  // namespace watch_list_app::server

#endif  // HANDLER_INDEX_HPP_
