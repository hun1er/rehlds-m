#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("HLDS Core"
  DESCRIPTION
    "Business logic of HLDS Launcher"
)

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "core")
set(TARGET_ALIAS "Core::${TARGET_NAME}")

add_library("${TARGET_NAME}")
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

set(HPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/include/${TARGET_NAME}")
set(CPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/src/${TARGET_NAME}")

target_sources("${TARGET_NAME}"
  PUBLIC
    "${CPP_SOURCES_DIR}/exports.cpp"
    "${HPP_SOURCES_DIR}/cmdline_args.hpp"
    "${HPP_SOURCES_DIR}/cmdline_processor.hpp"
    "${HPP_SOURCES_DIR}/init.hpp"

  PRIVATE
    "${CPP_SOURCES_DIR}/cmdline_args.cpp"
    "${CPP_SOURCES_DIR}/cmdline_processor.cpp"
    "${CPP_SOURCES_DIR}/init.cpp"
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
    "${LIB_COMMON}"
    "${LIB_MODEL}"
    "${LIB_UTIL}"
    "${LIB_VIEW}"

    $<$<PLATFORM_ID:Windows>:
      "${LIB_WINSOCK}"
    >
)

#-------------------------------------------------------------------------------
# Unit Tests
#-------------------------------------------------------------------------------

if(BUILD_UNIT_TESTS)
  add_subdirectory("tests")
endif()
