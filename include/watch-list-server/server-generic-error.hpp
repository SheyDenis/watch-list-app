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

namespace watch_list_app::server {

class ServerGenericError {
 private:
  std::string error_;
  std::optional<std::string> ex_;
  std::optional<int> os_error_;

 public:
  ServerGenericError(std::string error, std::optional<std::string> ex, std::optional<int> os_error)
      : error_(std::move(error)), ex_(std::move(ex)), os_error_(os_error) {}
  explicit ServerGenericError(std::string error) : ServerGenericError(std::move(error), std::nullopt, std::nullopt) {}
  ServerGenericError(std::string error, std::string ex) : ServerGenericError(std::move(error), std::move(ex), std::nullopt) {}
  ServerGenericError(std::string error, int os_error) : ServerGenericError(std::move(error), std::nullopt, os_error) {}

  explicit operator std::string() const {
    if (ServerConstants::include_debug_data()) {
      return fmt::format(
          FMT_STRING("{} [ex={}][os_error={}]"), error_, ex_.has_value() ? *ex_ : "N/A", os_error_.has_value() ? *os_error_ : 0);
    }
    return error_;
  }
};

typedef std::optional<ServerGenericError> OptionalServerGenericError;
template <typename V>
using ServerGenericErrorVariant = std::variant<ServerGenericError, V>;

}  // namespace watch_list_app::server

#endif  // SERVER_GENERIC_ERROR_HPP_
