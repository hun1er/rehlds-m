cmake_minimum_required(VERSION 3.21...3.27)

#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Util")

#-------------------------------------------------------------------------------
# Variable Definitions
#-------------------------------------------------------------------------------

set(PNAME_LOWER "util")
set(PNAME_CAPITALIZED "Util")

#-------------------------------------------------------------------------------
# Library Definition
#-------------------------------------------------------------------------------

add_library("${PNAME_LOWER}")
add_library("${PNAME_CAPITALIZED}::${PNAME_LOWER}" ALIAS "${PNAME_LOWER}")

# Common sources
target_sources("${PNAME_LOWER}"
  PRIVATE
    "circular_buffer.hpp"
    "console.hpp"
    "file.cpp"
    "file.hpp"
    "lifecycle.cpp"
    "lifecycle.hpp"
    "logger.cpp"
    "logger.hpp"
    "observable.hpp"
    "signal.hpp"
    "singleton.hpp"
    "string.hpp"
    "system.cpp"
    "system.hpp"
    "threadsafe_queue.hpp"
)

if(WIN32) # Windows sources
  target_sources("${PNAME_LOWER}"
    PRIVATE
      "windows/console.cpp"
      "windows/console.hpp"
      "windows/system/crash_dumper.cpp"
      "windows/system/crash_dumper.hpp"
      "windows/system/error.cpp"
      "windows/system/error.hpp"
      "windows/system/gdi.cpp"
      "windows/system/gdi.hpp"
      "windows/system/module.hpp"
      "windows/system/ntdll.cpp"
      "windows/system/ntdll.hpp"
      "windows/system/process.cpp"
      "windows/system/process.hpp"
  )
else() # Unix sources
  target_sources("${PNAME_LOWER}"
    PRIVATE
      "linux/console.cpp"
      "linux/console.hpp"
      "linux/signal.cpp"
      "linux/signal.hpp"
      "linux/system/error.cpp"
      "linux/system/error.hpp"
      "linux/system/module.hpp"
      "linux/system/process.hpp"
  )
endif()

target_include_directories("${PNAME_LOWER}"
  PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
)

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${PNAME_LOWER}"
  PUBLIC
    "Abstract::abstract"
    "fmt::fmt"
    "spdlog::spdlog"
    "StringPy::stringpy"

    $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
      "c"
      "stdc++fs"
      "${CMAKE_DL_LIBS}"
    >
)

if(WIN32)
  if(NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC" OR NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options("spdlog"
      PRIVATE
        # Because some idiot added the /MP compilation flag...
        -Wno-unused-command-line-argument
    )
  endif()
endif()
