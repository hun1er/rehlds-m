#-------------------------------------------------------------------------------
# Variable Definitions
#-------------------------------------------------------------------------------

set(PROJECT_NAME_CAP "ConsoleView")
set(PROJECT_NAME_LOW "console_view")

#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Console View")

#-------------------------------------------------------------------------------
# Library Definition
#-------------------------------------------------------------------------------

add_library("${PROJECT_NAME_LOW}")
add_library("View::console" ALIAS "${PROJECT_NAME_LOW}")

target_sources("${PROJECT_NAME_LOW}"
  PRIVATE
    "console_input.cpp"
    "console_input.hpp"
    "console_view.hpp"
)

if(WIN32) # Windows sources
  target_sources("${PROJECT_NAME_LOW}"
    PRIVATE
      "windows/console_input.cpp"
      "windows/console_view.cpp"
      "windows/console_view.hpp"
  )
else() # Unix sources
  target_sources("${PROJECT_NAME_LOW}"
    PRIVATE
      "linux/console_input.cpp"
      "linux/console_view.hpp"
      "linux/terminal_settings.hpp"
      "linux/tty_redirect.hpp"
  )
endif()

target_include_directories("${PROJECT_NAME_LOW}"
  PUBLIC
    "${CMAKE_SOURCE_DIR}/src/view"
)

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${PROJECT_NAME_LOW}"
  PUBLIC
    "${LIB_ABSTRACT}"
    "${LIB_FMT}"
    "${LIB_THREADS}"
    "${LIB_UTIL}"
)
