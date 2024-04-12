/**
 * @file database-builder.hpp
 * @author denis
 * @brief
 * @date 12024-02-12
 *
 */

#ifndef DATABASE_BUILDER_HPP_
#define DATABASE_BUILDER_HPP_

#include <fmt/core.h>

#include <memory>
#include <optional>

#include "watch-list-server/dal/database-json.hpp"
#include "watch-list-server/dal/database.hpp"
#include "watch-list-server/server-generic-error.hpp"
#include "watch-list-server/settings/server-settings-models.hpp"

namespace watch_list_app::server::dal {

class DatabaseBuilder {
 public:
  DatabaseBuilder() = delete;

  [[nodiscard]] static OptionalServerGenericError initialize(settings::ServerSettingsDatabase const& settings) {
    if (Database::instance_ != nullptr) {
      return ServerGenericError(fmt::format("Called [{}] multiple times", __PRETTY_FUNCTION__));
    }

    switch (settings.database_type) {
      case settings::ServerSettingsDatabase::DatabaseType::JSON:
        Database::instance_.reset(new DatabaseJSON(settings.database_path));
        break;
      default:
        __builtin_unreachable();
    }

    return Database::instance_->initialize();
  }
};

}  // namespace watch_list_app::server::dal

#endif  // DATABASE_BUILDER_HPP_
