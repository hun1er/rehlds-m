cmake_minimum_required(VERSION 3.21...3.27)

#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Model")

#-------------------------------------------------------------------------------
# Variable Definitions
#-------------------------------------------------------------------------------

set(PNAME_LOWER "model")
set(PNAME_CAPITALIZED "Model")

#-------------------------------------------------------------------------------
# Library Definition
#-------------------------------------------------------------------------------

add_library("${PNAME_LOWER}")
add_library("${PNAME_CAPITALIZED}::${PNAME_LOWER}" ALIAS "${PNAME_LOWER}")

# Common sources
target_sources("${PNAME_LOWER}"
  PRIVATE
    "cmdline_args.cpp"
    "cmdline_args.hpp"
    "console_commands.cpp"
    "console_commands.hpp"
    "server_loop.cpp"
    "server_loop.hpp"
    "server_status.hpp"
    "userinput_history.cpp"
    "userinput_history.hpp"
)

if(WIN32) # Windows sources
  target_sources("${PNAME_LOWER}"
    PRIVATE
      "windows/server_loop.cpp"
  )
else() # Unix sources
  target_sources("${PNAME_LOWER}"
    PRIVATE
      "linux/server_loop.cpp"
  )
endif()

target_compile_definitions("${PNAME_LOWER}"
  PRIVATE
    $<$<PLATFORM_ID:Windows>:
      NOMINMAX
    >
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
    "Common::common"
    "Threads::Threads"
    "Util::util"

    $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
      "c" "m"
    >
)
