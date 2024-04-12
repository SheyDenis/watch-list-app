/**
 * @file server-generic-error.cpp
 * @author denis
 * @brief
 * @date 12024-02-17
 *
 */

#include "watch-list-server/server-generic-error.hpp"

#include <fmt/core.h>

namespace watch_list_app::server {

std::string to_string(ServerGenericError const& err) {
  if (ServerConstants::include_debug_data()) {
    std::string os_error_str = "N/A";

    if (err.os_error.has_value()) {
      os_error_str = fmt::format("{} {}", *err.os_error, strerror(*err.os_error));
    }

    return fmt::format(FMT_STRING("{} [ex={}][os_error={}]"), err.error, err.ex.has_value() ? *err.ex : "N/A", os_error_str);
  }
  return err.error;
}

}  // namespace watch_list_app::server
