#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Util")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "util")
set(TARGET_ALIAS "Util::${TARGET_NAME}")

add_library("${TARGET_NAME}")
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

set(HPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/include/${TARGET_NAME}")
set(CPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/src/${TARGET_NAME}")

target_sources("${TARGET_NAME}"
  PUBLIC
    "${HPP_SOURCES_DIR}/circular_buffer.hpp"
    "${HPP_SOURCES_DIR}/console.hpp"
    "${HPP_SOURCES_DIR}/file.hpp"
    "${HPP_SOURCES_DIR}/lifecycle.hpp"
    "${HPP_SOURCES_DIR}/log_output.hpp"
    "${HPP_SOURCES_DIR}/logger.hpp"
    "${HPP_SOURCES_DIR}/observable.hpp"
    "${HPP_SOURCES_DIR}/signal.hpp"
    "${HPP_SOURCES_DIR}/singleton.hpp"
    "${HPP_SOURCES_DIR}/string.hpp"
    "${HPP_SOURCES_DIR}/system.hpp"
    "${HPP_SOURCES_DIR}/threadsafe_queue.hpp"

  PRIVATE
    "${CPP_SOURCES_DIR}/file.cpp"
    "${CPP_SOURCES_DIR}/lifecycle.cpp"
    "${CPP_SOURCES_DIR}/logger.cpp"
    "${CPP_SOURCES_DIR}/system.cpp"
)

if(WIN32)
  target_sources("${TARGET_NAME}"
    PUBLIC
      "${HPP_SOURCES_DIR}/windows/console.hpp"
      "${HPP_SOURCES_DIR}/windows/system/crash_dumper.hpp"
      "${HPP_SOURCES_DIR}/windows/system/error.hpp"
      "${HPP_SOURCES_DIR}/windows/system/gdi.hpp"
      "${HPP_SOURCES_DIR}/windows/system/module.hpp"
      "${HPP_SOURCES_DIR}/windows/system/ntdll.hpp"
      "${HPP_SOURCES_DIR}/windows/system/process.hpp"

    PRIVATE
      "${CPP_SOURCES_DIR}/windows/console.cpp"
      "${CPP_SOURCES_DIR}/windows/system/crash_dumper.cpp"
      "${CPP_SOURCES_DIR}/windows/system/error.cpp"
      "${CPP_SOURCES_DIR}/windows/system/gdi.cpp"
      "${CPP_SOURCES_DIR}/windows/system/ntdll.cpp"
      "${CPP_SOURCES_DIR}/windows/system/process.cpp"
  )
else()
  target_sources("${TARGET_NAME}"
    PUBLIC
      "${HPP_SOURCES_DIR}/linux/console.hpp"
      "${HPP_SOURCES_DIR}/linux/signal.hpp"
      "${HPP_SOURCES_DIR}/linux/system/error.hpp"
      "${HPP_SOURCES_DIR}/linux/system/module.hpp"
      "${HPP_SOURCES_DIR}/linux/system/process.hpp"

    PRIVATE
      "${CPP_SOURCES_DIR}/linux/console.cpp"
      "${CPP_SOURCES_DIR}/linux/signal.cpp"
      "${CPP_SOURCES_DIR}/linux/system/error.cpp"
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
# Compiler Flags
#-------------------------------------------------------------------------------

if(WIN32)
  if(NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC" OR NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options("spdlog"
      PRIVATE
        # Because someone added the /MP compilation flag
        -Wno-unused-command-line-argument
    )
  endif()
endif()

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${TARGET_NAME}"
  PUBLIC
    "${LIB_FMT}"
    "${LIB_SPDLOG}"
    "${LIB_STRINGPY}"

    $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
      "${LIB_C}"
      "${LIB_STDC_FS}"
      "${CMAKE_DL_LIBS}"
    >
)

#-------------------------------------------------------------------------------
# Unit Tests
#-------------------------------------------------------------------------------

if(BUILD_UNIT_TESTS)
  add_subdirectory("tests")
endif()
