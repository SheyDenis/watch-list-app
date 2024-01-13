FIND_PACKAGE(Catch2 REQUIRED)
FIND_PACKAGE(trompeloeil REQUIRED)

ADD_EXECUTABLE(dummy-tests
               src/main.cpp
)

SET_CMAKE_TARGET_PROPERTIES(dummy-tests)

TARGET_LINK_LIBRARIES(dummy-tests
                      PUBLIC
                      Catch2::Catch2
                      trompeloeil::trompeloeil
)
