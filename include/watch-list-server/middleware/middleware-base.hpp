/**
 * @file middleware-base.hpp
 * @author denis
 * @brief
 * @date 12024-04-13
 *
 */

#ifndef MIDDLEWARE_BASE_HPP_
#define MIDDLEWARE_BASE_HPP_
#include "watch-list-server/server-logger.hpp"

namespace watch_list_app::server::middleware {

class MiddlewareBase {
 protected:
  ServerLogger logger_;
  std::string const middleware_name_;

 protected:
  MiddlewareBase(std::string middleware_name) : logger_(middleware_name), middleware_name_(std::move(middleware_name)) {}
};

}  // namespace watch_list_app::server::middleware

#endif  // MIDDLEWARE_BASE_HPP_
