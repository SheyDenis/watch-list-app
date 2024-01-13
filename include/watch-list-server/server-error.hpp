/**
 * @file server-error.hpp
 * @author denis
 * @brief
 * @date 12024-01-12
 *
 */

#ifndef SERVER_ERROR_HPP_
#define SERVER_ERROR_HPP_

#include <fmt/core.h>

#include <optional>
#include <stdexcept>
#include <string>

namespace watch_list_app::server {

template <typename T>
std::string format_error(T const& err) {
  return to_string(err);
}

template <typename T>
std::string format_error(std::optional<T> const& err) {
  if (!err.has_value()) {
    throw std::runtime_error(fmt::format("Called [{}] with [std::nullopt] value", __PRETTY_FUNCTION__));
  }
  return format_error<T>(*err);
}

}  // namespace watch_list_app::server

#endif  // SERVER_ERROR_HPP_
