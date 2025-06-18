#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SFEngine::SFEngine" for configuration "Debug"
set_property(TARGET SFEngine::SFEngine APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SFEngine::SFEngine PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/SFEngine.lib"
  )

list(APPEND _cmake_import_check_targets SFEngine::SFEngine )
list(APPEND _cmake_import_check_files_for_SFEngine::SFEngine "${_IMPORT_PREFIX}/lib/SFEngine.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
