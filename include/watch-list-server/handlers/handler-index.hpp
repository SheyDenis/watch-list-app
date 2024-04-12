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

namespace watch_list_app::server::handlers {

class HandlerIndex : public HandlerBase {
 private:
  friend struct HandlerInstance<HandlerIndex>;

 protected:
  HandlerIndex();

 public:
  ~HandlerIndex() override = default;

  [[nodiscard]] OptionalHandlerError handle_get_impl(httplib::Request const& req, httplib::Response& res) override;
};

}  // namespace watch_list_app::server::handlers

#endif  // HANDLER_INDEX_HPP_
