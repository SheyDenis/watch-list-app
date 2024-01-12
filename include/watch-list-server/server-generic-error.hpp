/**
 * @file server-error.hpp
 * @author denis
 * @brief
 * @date 12024-01-04
 *
 */

#ifndef SERVER_GENERIC_ERROR_HPP_
#define SERVER_GENERIC_ERROR_HPP_

#include <fmt/format.h>

#include <optional>
#include <string>
#include <utility>
#include <variant>

#include "watch-list-server/server-constants.hpp"
#include "watch-list-server/server-error.hpp"

namespace watch_list_app::server {

struct ServerGenericError {
  /// @brief A generic error type used to convey general runtime error has occurred.

  std::string const error;
  std::optional<std::string> const ex = std::nullopt;
  std::optional<int> const os_error = std::nullopt;

  explicit ServerGenericError(std::string _error,
                              std::optional<std::string> _ex = std::nullopt,
                              std::optional<int> _os_error = std::nullopt)
      : error(std::move(_error)), ex(std::move(_ex)), os_error(_os_error) {}
};
typedef std::optional<ServerGenericError> OptionalServerGenericError;
template <typename V>
using ServerGenericErrorVariant = std::variant<ServerGenericError, V>;

template <>
struct ErrorFormatter<ServerGenericError> {
  static std::string to_string(ServerGenericError const& err) {
    if (ServerConstants::include_debug_data()) {
      std::string os_error_str = "N/A";

      if (err.os_error.has_value()) {
        os_error_str = fmt::format("{} {}", *err.os_error, strerror(*err.os_error));
      }

      return fmt::format(FMT_STRING("{} [ex={}][os_error={}]"), err.error, err.ex.has_value() ? *err.ex : "N/A", os_error_str);
    }
    return err.error;
  }
};

}  // namespace watch_list_app::server

template <>
struct fmt::formatter<watch_list_app::server::ServerGenericError> : fmt::formatter<std::string> {
  auto format(watch_list_app::server::ServerGenericError const& v, format_context& ctx) const -> decltype(ctx.out()) {
    return format_to(ctx.out(), watch_list_app::server::format_error(v));
  }
};

#endif  // SERVER_GENERIC_ERROR_HPP_
