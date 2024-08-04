#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Model")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "model")
set(TARGET_ALIAS "Model::${TARGET_NAME}")

add_library("${TARGET_NAME}")
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

set(HPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/include/${TARGET_NAME}")
set(CPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/src/${TARGET_NAME}")

target_sources("${TARGET_NAME}"
  PUBLIC
    "${HPP_SOURCES_DIR}/console_commands.hpp"
    "${HPP_SOURCES_DIR}/server_loop.hpp"
    "${HPP_SOURCES_DIR}/server_status.hpp"
    "${HPP_SOURCES_DIR}/userinput_history.hpp"

  PRIVATE
    "${CPP_SOURCES_DIR}/console_commands.cpp"
    "${CPP_SOURCES_DIR}/server_loop.cpp"
    "${CPP_SOURCES_DIR}/userinput_history.cpp"
)

if(WIN32)
  target_sources("${TARGET_NAME}"
    PRIVATE
      "${CPP_SOURCES_DIR}/windows/server_loop.cpp"
  )
else()
  target_sources("${TARGET_NAME}"
    PRIVATE
      "${CPP_SOURCES_DIR}/linux/server_loop.cpp"
  )
endif()

#-------------------------------------------------------------------------------
# Include Directories
#-------------------------------------------------------------------------------

target_include_directories("${TARGET_NAME}"
  PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

#-------------------------------------------------------------------------------
# Compile Definitions
#-------------------------------------------------------------------------------

target_compile_definitions("${TARGET_NAME}"
  PRIVATE
    $<$<PLATFORM_ID:Windows>:
      NOMINMAX
    >
)

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${TARGET_NAME}"
  PUBLIC
    "${LIB_THREADS}"
    "${LIB_COMMON}"
    "${LIB_UTIL}"

    $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
      "${LIB_C}"
      "${LIB_MATH}"
    >
)

#-------------------------------------------------------------------------------
# Unit Tests
#-------------------------------------------------------------------------------

if(BUILD_UNIT_TESTS)
  add_subdirectory("tests")
endif()
