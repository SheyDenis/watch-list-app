/**
 * @file server-logger.cpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#include "watch-list-server/server-logger.hpp"

namespace watch_list_app::server {
spdlog::level::level_enum ServerLogger::default_level_ = spdlog::level::level_enum::info;

}
