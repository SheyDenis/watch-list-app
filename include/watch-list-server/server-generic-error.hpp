/**
 * @file server-error.hpp
 * @author denis
 * @brief
 * @date 12024-01-04
 *
 */

#ifndef SERVER_GENERIC_ERROR_HPP_
#define SERVER_GENERIC_ERROR_HPP_

#include <optional>
#include <string>
#include <utility>
#include <variant>

#include "watch-list-server/server-constants.hpp"

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
using OptionalServerGenericError = std::optional<ServerGenericError>;
template <typename V>
using ServerGenericErrorVariant = std::variant<ServerGenericError, V>;

std::string to_string(ServerGenericError const& err);

}  // namespace watch_list_app::server

#endif  // SERVER_GENERIC_ERROR_HPP_
