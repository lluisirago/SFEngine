function(enable_compiler_warnings TARGET)
    if (TARGET ${TARGET})
        if (MSVC)
            target_compile_options(${TARGET} PRIVATE /W4 /permissive-)
        else()
            target_compile_options(${TARGET} PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Wno-unused-parameter
            )
        endif()
    endif()
endfunction()
