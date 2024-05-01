/**
 * @file http-utils.hpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#ifndef HTTP_UTILS_HPP_
#define HTTP_UTILS_HPP_

#include <cstdint>
#include <string_view>

namespace watch_list_app::server {

// FIXME - Use Pistache instead.
enum class HTTPMethod : std::uint8_t {
  HTTP_DELETE,
  HTTP_GET,
  HTTP_HEAD,
  HTTP_PATCH,
  HTTP_POST,
  HTTP_PUT,
};

template <typename T = std::string_view>
static T http_method_to_string(HTTPMethod method) {
  switch (method) {
    case HTTPMethod::HTTP_DELETE:
      return "DELETE";
    case HTTPMethod::HTTP_GET:
      return "GET";
    case HTTPMethod::HTTP_HEAD:
      return "HEAD";
    case HTTPMethod::HTTP_PATCH:
      return "PATCH";
    case HTTPMethod::HTTP_POST:
      return "POST";
    case HTTPMethod::HTTP_PUT:
      return "PUT";
  }

  __builtin_unreachable();
}

}  // namespace watch_list_app::server

#endif  // HTTP_UTILS_HPP_
