# ============================
# Create the library target
# ============================
set(SFENGINE_LIBRARY_TYPE STATIC)
if(SFENGINE_BUILD_SHARED)
    set(SFENGINE_LIBRARY_TYPE SHARED)
endif()

add_library(SFEngine ${SFENGINE_LIBRARY_TYPE})
add_library(SFEngine::SFEngine ALIAS SFEngine)

# ============================
# Define include directories
# ============================
target_include_directories(SFEngine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor>
        $<INSTALL_INTERFACE:include>
)

# ============================
# Add source files
# ============================
file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
)

file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

target_sources(SFEngine PRIVATE ${ENGINE_SRC} ${ENGINE_HEADERS})


# ============================
# Link SFML as required dependency
# ============================
target_link_libraries(SFEngine
    PRIVATE
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
)

# ============================
# Definitions to export/import symbols in build SHARED
# ============================
if(SFENGINE_BUILD_SHARED)
    target_compile_definitions(SFEngine
        PRIVATE
            SFENGINE_SHARED    # Indica que se construye/usa la librería compartida
            SFENGINE_EXPORTS   # Solo para la compilación del motor (exporta símbolos)
    )
endif()

# ============================
# Enable warnings/sanitizers if configured from root
# ============================
if(SFENGINE_ENABLE_WARNINGS)
    enable_compiler_warnings(TARGET SFEngine)
endif()

if(SFENGINE_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_sanitizers(TARGET SFEngine)
endif()
