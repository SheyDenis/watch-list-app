MACRO(INCLUDE_WHAT_YOU_USE __target)
    IF (NOT DEFINED ${IWYU_PATH})
        FIND_PROGRAM(IWYU_PATH NAMES include-what-you-use iwyu REQUIRED)

        IF (${IS_DEV})
            SET(IWYU_ERR 0)
        ELSE ()
            SET(IWYU_ERR 1)
        ENDIF (${IS_DEV})

        SET(IWYU_PATH_AND_OPTIONS ${IWYU_PATH} -Xiwyu --error=${IWYU_ERR} -Xiwyu --no_fwd_decls -Xiwyu --transitive_includes_only -Xiwyu --cxx17ns)
        UNSET(IWYU_ERR)
    ENDIF ()

    SET_PROPERTY(TARGET ${__target} PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH_AND_OPTIONS})
ENDMACRO()
