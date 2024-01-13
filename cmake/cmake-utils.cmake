SET(DEFAULT_COMPILER_WARNING_FLAGS
    -Werror=all
    -Werror=extra
    -Werror=narrowing
    -Werror=reorder
    -Werror=shadow
    -Werror=unused
    -Werror=unused-const-variable
    -Werror=unused-parameter

    $<$<BOOL:${IS_DEV}>:-Wno-unused-const-variable>
    $<$<BOOL:${IS_DEV}>:-Wno-unused-parameter>
    $<$<BOOL:${IS_DEV}>:-Wno-unused>
)
MACRO(SET_CMAKE_TARGET_PROPERTIES __target)
    SET_TARGET_PROPERTIES(${__target} PROPERTIES CXX_STANDARD 17 POSITION_INDEPENDENT_CODE ON)
ENDMACRO()
