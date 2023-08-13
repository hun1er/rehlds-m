cmake_minimum_required(VERSION 3.21...3.27)

#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Console View")

#-------------------------------------------------------------------------------
# Variable Definitions
#-------------------------------------------------------------------------------

set(PNAME_LOWER "console_view")

#-------------------------------------------------------------------------------
# Library Definition
#-------------------------------------------------------------------------------

add_library("${PNAME_LOWER}")
add_library("View::console" ALIAS "${PNAME_LOWER}")

target_sources("${PNAME_LOWER}"
  PRIVATE
    "console_input.cpp"
    "console_input.hpp"
    "console_view.hpp"
)

if(WIN32) # Windows sources
  target_sources("${PNAME_LOWER}"
    PRIVATE
      "windows/console_input.cpp"
      "windows/console_view.cpp"
      "windows/console_view.hpp"
  )
else() # Unix sources
  target_sources("${PNAME_LOWER}"
    PRIVATE
      "linux/console_input.cpp"
      "linux/console_view.hpp"
      "linux/terminal_settings.hpp"
      "linux/tty_redirect.hpp"
  )
endif()

target_include_directories("${PNAME_LOWER}"
  PUBLIC
    "${CMAKE_SOURCE_DIR}/src/view"
)

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${PNAME_LOWER}"
  PUBLIC
    "Abstract::abstract"
    "fmt::fmt"
    "Threads::Threads"
    "Util::util"
)
