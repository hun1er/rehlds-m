#-------------------------------------------------------------------------------
# Variable definitions
#-------------------------------------------------------------------------------

set(PROJECT_NAME_CAP "Util")
set(PROJECT_NAME_LOW "util")

#-------------------------------------------------------------------------------
# Project definition
#-------------------------------------------------------------------------------

project("${PROJECT_NAME_CAP}")

#-------------------------------------------------------------------------------
# Library definition
#-------------------------------------------------------------------------------

add_library("${PROJECT_NAME_LOW}")
add_library("${PROJECT_NAME_CAP}::${PROJECT_NAME_LOW}" ALIAS "${PROJECT_NAME_LOW}")

# Common sources
target_sources("${PROJECT_NAME_LOW}"
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
  target_sources("${PROJECT_NAME_LOW}"
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
  target_sources("${PROJECT_NAME_LOW}"
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

target_include_directories("${PROJECT_NAME_LOW}"
  PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
)

#-------------------------------------------------------------------------------
# Link libraries
#-------------------------------------------------------------------------------

target_link_libraries("${PROJECT_NAME_LOW}"
  PUBLIC
    "${LIB_ABSTRACT}"
    "${LIB_FMT}"
    "${LIB_SPDLOG}"
    "${LIB_STRINGPY}"

    $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
      "${LIB_C}"
      "${LIB_STDC_FS}"
      "${CMAKE_DL_LIBS}"
    >
)

if(WIN32)
  if(NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC" OR NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options("spdlog"
      PRIVATE
        # Because someone added the /MP compilation flag
        -Wno-unused-command-line-argument
    )
  endif()
endif()
