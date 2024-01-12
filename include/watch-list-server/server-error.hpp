/**
 * @file server-error.hpp
 * @author denis
 * @brief
 * @date 12024-01-12
 *
 */

#ifndef SERVER_ERROR_HPP_
#define SERVER_ERROR_HPP_

#include <string>

namespace watch_list_app::server {

template <class T>
struct ErrorFormatter {
  static_assert(false, "Do not use base template");
  static std::string to_string(T const& err);
};

template <class T>
std::string format_error(T const& err) {
  return ErrorFormatter<T>::to_string(err);
}

}  // namespace watch_list_app::server

#endif  // SERVER_ERROR_HPP_
