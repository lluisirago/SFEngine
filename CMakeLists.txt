cmake_minimum_required(VERSION 3.21)

# ============================
# Project Definition
# ============================
project(SFEngine
    VERSION 0.1.0
    DESCRIPTION "Modular C++ game engine based on SFML"
    LANGUAGES CXX
)

# ============================
# Set C++ Standard
# ============================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================
# Project Options
# ============================
option(SFENGINE_BUILD_SHARED "Build SFEngine as a shared library" OFF)
option(SFENGINE_BUILD_EXAMPLES "Build example sandbox app" ON)
option(SFENGINE_ENABLE_WARNINGS "Enable compiler warnings" ON)
option(SFENGINE_ENABLE_CLANG_TOOLS "Enable clang-tidy and format targets" ON)
option(SFENGINE_ENABLE_SANITIZERS "Enable sanitizers in Debug mode" OFF)

# ============================
# Paths
# ============================
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ============================
# Include CMake Modules
# ============================
include(CompilerWarnings)
include(ClangTools)
include(Sanitizers)
include(FetchDependencies)

# ============================
# Dependencies
# ============================
find_package(SFML 2.6.1 COMPONENTS system window graphics audio REQUIRED)
fetch_json()
fetch_zlib()

# ============================
# Engine Target
# ============================
add_subdirectory(engine)

# ============================
# Examples
# ============================

if(SFENGINE_BUILD_EXAMPLES)
    # add_subdirectory(examples/sandbox)
endif()

# ============================
# Clang Tools
# ============================
if(SFENGINE_ENABLE_CLANG_TOOLS)
    enable_clang_tools()
endif()

# ============================
# Warnings and Sanitizers
# ============================
if(SFENGINE_ENABLE_WARNINGS AND TARGET SFEngine)
    enable_compiler_warnings(SFEngine)
endif()

if(SFENGINE_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_sanitizers(TARGET SFEngine)
endif()

# ============================
# Installation
# ============================
install(TARGETS SFEngine EXPORT SFEngineTargets)
install(DIRECTORY engine/include/ DESTINATION include)
install(DIRECTORY engine/vendor/ DESTINATION include/vendor)
install(EXPORT SFEngineTargets
    FILE SFEngineTargets.cmake
    NAMESPACE SFEngine::
    DESTINATION lib/cmake/SFEngine
) 

# ============================
# Package Export
# ============================
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/SFEngineConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/SFEngineConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/SFEngineConfig.cmake
  INSTALL_DESTINATION lib/cmake/SFEngine
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/SFEngineConfigVersion.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/SFEngineConfig.cmake
        DESTINATION lib/cmake/SFEngine
)

# ============================
# Summary Output
# ============================
message(STATUS "SFEngine build configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build examples: ${SFENGINE_BUILD_EXAMPLES}")
message(STATUS "  Enable clang tools: ${SFENGINE_ENABLE_CLANG_TOOLS}")
message(STATUS "  Enable warnings: ${SFENGINE_ENABLE_WARNINGS}")
message(STATUS "  Enable sanitizers: ${SFENGINE_ENABLE_SANITIZERS}")
