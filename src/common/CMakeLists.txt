#-------------------------------------------------------------------------------
# Variable definitions
#-------------------------------------------------------------------------------

set(PROJECT_NAME_CAP "Common")
set(PROJECT_NAME_LOW "common")

#-------------------------------------------------------------------------------
# Project definition
#-------------------------------------------------------------------------------

project("${PROJECT_NAME_CAP}")

#-------------------------------------------------------------------------------
# Library definition
#-------------------------------------------------------------------------------

add_library("${PROJECT_NAME_LOW}")
add_library("${PROJECT_NAME_CAP}::${PROJECT_NAME_LOW}" ALIAS "${PROJECT_NAME_LOW}")

target_sources("${PROJECT_NAME_LOW}"
  PRIVATE
    "engine/engine_wrapper.hpp"
    "engine/interface/dedicated_serverapi_interface.hpp"
    "engine/interface/system_interface.hpp"
    "engine/interface/systemmodule_interface.hpp"
    "filesystem/filesystem_wrapper.hpp"
    "filesystem/interface/filesystem_interface.hpp"
    "hlds/interface/hlds_exports.hpp"
    "vgui/interface/vguimodule_interface.hpp"
    "interface.cpp"
    "interface.hpp"
    "module_wrapper.cpp"
    "module_wrapper.hpp"
    "object_container.hpp"
    "object_list.cpp"
    "object_list.hpp"
    "platform.hpp"
)

target_include_directories("${PROJECT_NAME_LOW}"
  PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
)

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${PROJECT_NAME_LOW}"
  PUBLIC
    "${LIB_UTIL}"

    $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
      "${LIB_C}"
    >
)
