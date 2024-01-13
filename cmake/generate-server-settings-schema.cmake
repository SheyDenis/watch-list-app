FILE(READ ${CMAKE_CURRENT_LIST_DIR}/../config/settings-schema.json SERVER_SETTINGS_SCHEMA_INPUT)
CONFIGURE_FILE(${CMAKE_CURRENT_LIST_DIR}/../include/watch-list-server/server-settings-schema.hpp.in ${CMAKE_CURRENT_LIST_DIR}/../include/watch-list-server/server-settings-schema.hpp)
