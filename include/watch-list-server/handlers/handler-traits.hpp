/**
 * @file handler-traits.hpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#ifndef HANDLER_TRAITS_HPP_
#define HANDLER_TRAITS_HPP_

#include <httplib.h>

#include <type_traits>

namespace watch_list_app::server::handlers {

template <class T>
struct HandlerInstance {
  static T& instance() {
    static T instance;
    return instance;
  }
};

template <typename T>
struct HandlerTraits {
  using HandlerType = T;

  static void handle_delete(httplib::Request const& req, httplib::Response& res) {
    handlers::HandlerInstance<HandlerType>::instance().handle_delete(req, res);
  }
  static void handle_get(httplib::Request const& req, httplib::Response& res) {
    handlers::HandlerInstance<HandlerType>::instance().handle_get(req, res);
  }
  static void handle_patch(httplib::Request const& req, httplib::Response& res) {
    handlers::HandlerInstance<HandlerType>::instance().handle_patch(req, res);
  }
  static void handle_post(httplib::Request const& req, httplib::Response& res) {
    handlers::HandlerInstance<HandlerType>::instance().handle_post(req, res);
  }
  static void handle_put(httplib::Request const& req, httplib::Response& res) {
    handlers::HandlerInstance<HandlerType>::instance().handle_put(req, res);
  }
};

}  // namespace watch_list_app::server::handlers

#endif  // HANDLER_TRAITS_HPP_
