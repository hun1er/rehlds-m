#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("View")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "view")
set(TARGET_ALIAS "View::${TARGET_NAME}")

add_library("${TARGET_NAME}")
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

set(HPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/include/${TARGET_NAME}")
set(CPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/src/${TARGET_NAME}")

target_sources("${TARGET_NAME}"
  PUBLIC
    "${HPP_SOURCES_DIR}/base_view.hpp"
    "${HPP_SOURCES_DIR}/console_input.hpp"
    "${HPP_SOURCES_DIR}/console_view.hpp"

  PRIVATE
    "${CPP_SOURCES_DIR}/console_input.cpp"
)

if(WIN32)
  target_sources("${TARGET_NAME}"
    PUBLIC
      "${HPP_SOURCES_DIR}/windows/console_view.hpp"

    PRIVATE
      "${CPP_SOURCES_DIR}/windows/console_input.cpp"
      "${CPP_SOURCES_DIR}/windows/console_view.cpp"
  )
else()
  target_sources("${TARGET_NAME}"
    PUBLIC
      "${HPP_SOURCES_DIR}/linux/console_view.hpp"
      "${HPP_SOURCES_DIR}/linux/terminal_settings.hpp"
      "${HPP_SOURCES_DIR}/linux/tty_redirect.hpp"

    PRIVATE
      "${CPP_SOURCES_DIR}/linux/console_input.cpp"
  )
endif()

#-------------------------------------------------------------------------------
# Include Directories
#-------------------------------------------------------------------------------

target_include_directories("${TARGET_NAME}"
  PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries("${TARGET_NAME}"
  PUBLIC
    "${LIB_FMT}"
    "${LIB_THREADS}"
    "${LIB_PRESENTER}"
    "${LIB_UTIL}"
)
