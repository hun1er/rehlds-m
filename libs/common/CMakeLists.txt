#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Common")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "common")
set(TARGET_ALIAS "Common::${TARGET_NAME}")

add_library("${TARGET_NAME}")
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

set(HPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/include/${TARGET_NAME}")
set(CPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/src/${TARGET_NAME}")

target_sources("${TARGET_NAME}"
  INTERFACE
    "$<$<PLATFORM_ID:Windows>:${PROJECT_SOURCE_DIR}/exports.def>"

  PUBLIC
    "${HPP_SOURCES_DIR}/engine/engine_wrapper.hpp"
    "${HPP_SOURCES_DIR}/engine/interface/dedicated_serverapi_interface.hpp"
    "${HPP_SOURCES_DIR}/engine/interface/system_interface.hpp"
    "${HPP_SOURCES_DIR}/engine/interface/systemmodule_interface.hpp"
    "${HPP_SOURCES_DIR}/filesystem/filesystem_wrapper.hpp"
    "${HPP_SOURCES_DIR}/filesystem/interface/filesystem_interface.hpp"
    "${HPP_SOURCES_DIR}/hlds/interface/hlds_exports.hpp"
    "${HPP_SOURCES_DIR}/vgui/interface/vguimodule_interface.hpp"
    "${HPP_SOURCES_DIR}/interface.hpp"
    "${HPP_SOURCES_DIR}/module_wrapper.hpp"
    "${HPP_SOURCES_DIR}/object_container.hpp"
    "${HPP_SOURCES_DIR}/object_list.hpp"
    "${HPP_SOURCES_DIR}/platform.hpp"

  PRIVATE
    "${CPP_SOURCES_DIR}/interface.cpp"
    "${CPP_SOURCES_DIR}/module_wrapper.cpp"
    "${CPP_SOURCES_DIR}/object_list.cpp"
)

#-------------------------------------------------------------------------------
# Include Directories
#-------------------------------------------------------------------------------

target_include_directories("${TARGET_NAME}"
  PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${TARGET_NAME}"
  PUBLIC
    "${LIB_UTIL}"

    $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
      "${LIB_C}"
    >
)

#-------------------------------------------------------------------------------
# Unit Tests
#-------------------------------------------------------------------------------

if(BUILD_UNIT_TESTS)
  add_subdirectory("tests")
endif()
