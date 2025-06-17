function(enable_clang_tools)
    find_program(CLANG_FORMAT_BIN clang-format)
    find_program(CLANG_TIDY_BIN clang-tidy)

    if (CLANG_FORMAT_BIN)
        file(GLOB_RECURSE ALL_SOURCE_FILES CONFIGURE_DEPENDS
            ${CMAKE_SOURCE_DIR}/engine/include/*.h
            ${CMAKE_SOURCE_DIR}/engine/src/*.cpp
        )

        add_custom_target(format
            COMMAND ${CLANG_FORMAT_BIN}
            -i
            -style=file
            ${ALL_SOURCE_FILES}
            COMMENT "Running clang-format on engine source files"
        )
    endif()

    if (CLANG_TIDY_BIN)
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_BIN}")
    endif()
endfunction()
