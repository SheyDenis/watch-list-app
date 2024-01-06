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
  ServerGenericError(std::string error, std::optional<std::reference_wrapper<std::exception const>> ex, std::optional<int> os_error)
      : error_(std::move(error)), ex_(std::nullopt), os_error_(os_error) {
    if (ex.has_value()) {
      ex_ = ex->get().what();
    }
  }
  explicit ServerGenericError(std::string error) : ServerGenericError(std::move(error), std::nullopt, std::nullopt) {}
  ServerGenericError(std::string error, std::exception const& ex) : ServerGenericError(std::move(error), ex, std::nullopt) {}
  ServerGenericError(std::string error, int os_error) : ServerGenericError(std::move(error), std::nullopt, os_error) {}

  explicit operator std::string() const {
    if (ServerConstants::include_debug_data()) {
      std::string os_error_str = "0";

      if (os_error_.has_value()) {
        os_error_str = fmt::format("{} {}", *os_error_, strerror(*os_error_));
      }

      return fmt::format(FMT_STRING("{} [ex={}][os_error={}]"), error_, ex_.has_value() ? *ex_ : "N/A", os_error_str);
    }
    return error_;
  }
};

typedef std::optional<ServerGenericError> OptionalServerGenericError;
template <typename V>
using ServerGenericErrorVariant = std::variant<ServerGenericError, V>;

}  // namespace watch_list_app::server

template <>
struct fmt::formatter<watch_list_app::server::ServerGenericError> : fmt::formatter<std::string> {
  auto format(watch_list_app::server::ServerGenericError v, format_context& ctx) const -> decltype(ctx.out()) {
    return format_to(ctx.out(), std::string(v));
  }
};

#endif  // SERVER_GENERIC_ERROR_HPP_
