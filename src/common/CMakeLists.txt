cmake_minimum_required(VERSION 3.21...3.27)

#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Common")

#-------------------------------------------------------------------------------
# Variable Definitions
#-------------------------------------------------------------------------------

set(PNAME_LOWER "common")
set(PNAME_CAPITALIZED "Common")

#-------------------------------------------------------------------------------
# Library Definition
#-------------------------------------------------------------------------------

add_library("${PNAME_LOWER}")
add_library("${PNAME_CAPITALIZED}::${PNAME_LOWER}" ALIAS "${PNAME_LOWER}")

target_sources("${PNAME_LOWER}"
  PUBLIC
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

target_include_directories("${PNAME_LOWER}"
  PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
)

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${PNAME_LOWER}"
  PUBLIC
    "Util::util"

    $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
      "c"
    >
)
