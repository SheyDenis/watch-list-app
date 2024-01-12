CMAKE_MINIMUM_REQUIRED(VERSION 3.25)

PROJECT(WatchListApp VERSION 0.0.1 LANGUAGES CXX)

OPTION(IS_DEV "If enabled, disables some compile-time errors that make development difficult." OFF)

INCLUDE(GNUInstallDirs)

SET(EXEC_NAME ${PROJECT_NAME})
SET(CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_SOURCE_DIR}/build/build/${CMAKE_BUILD_TYPE}/generators
    ${CMAKE_MODULE_PATH}
)
INCLUDE(cmake-utils)
INCLUDE(iwyu)

FIND_PACKAGE(fmt REQUIRED)
FIND_PACKAGE(httplib REQUIRED)
FIND_PACKAGE(spdlog REQUIRED)
FIND_PACKAGE(RapidJSON REQUIRED)

ADD_EXECUTABLE(${EXEC_NAME}
               src/handlers/handler-base.cpp
               src/handlers/handler-health-check.cpp
               src/handlers/handler-index.cpp
               src/main.cpp
               src/server-listener.cpp
               src/server-logger.cpp
               src/settings/server-settings-loader.cpp
               src/settings/server-settings.cpp
)
SET_CMAKE_TARGET_PROPERTIES(${EXEC_NAME})

TARGET_INCLUDE_DIRECTORIES(${EXEC_NAME}
                           PUBLIC
                           ${PROJECT_SOURCE_DIR}/include
)

TARGET_LINK_DIRECTORIES(${EXEC_NAME}
                        PUBLIC
)

TARGET_LINK_LIBRARIES(${EXEC_NAME}
                      PUBLIC
                      fmt::fmt
                      httplib::httplib
                      rapidjson
                      spdlog::spdlog
)

TARGET_COMPILE_OPTIONS(${EXEC_NAME}
                       PRIVATE
                       ${DEFAULT_COMPILER_WARNING_FLAGS}
)

ADD_CUSTOM_COMMAND(TARGET ${EXEC_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -P ${PROJECT_SOURCE_DIR}/cmake/generate-server-settings-schema.cmake)
INCLUDE_WHAT_YOU_USE(${EXEC_NAME})

IF (NOT DISABLE_TESTS)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(unittests)
ENDIF ()
