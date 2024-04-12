MACRO(INCLUDE_WHAT_YOU_USE __target)
    IF (NOT DEFINED ${IWYU_PATH})
        FIND_PROGRAM(IWYU_PATH NAMES include-what-you-use iwyu REQUIRED)

        IF (IWYU_PATH)
            SET(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH})
        ELSE ()
            MESSAGE(FATAL_ERROR "IWYU not found. Please install it.")
        ENDIF ()

        IF (${IS_DEV})
            SET(IWYU_ERR 0)
        ELSE ()
            SET(IWYU_ERR 1)
        ENDIF (${IS_DEV})

        SET(IWYU_PATH_AND_OPTIONS ${IWYU_PATH} -Xiwyu --error=${IWYU_ERR} -Xiwyu --no_fwd_decls -Xiwyu --transitive_includes_only -Xiwyu --cxx17ns)
        UNSET(IWYU_ERR)
    ENDIF ()

    TARGET_INCLUDE_DIRECTORIES(${__target} PRIVATE ${CMAKE_CXX_INCLUDE_WHAT_YOU_USE})
    SET_PROPERTY(TARGET ${__target} PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH_AND_OPTIONS})
ENDMACRO()
