cmake_minimum_required(VERSION 3.21...3.27)

#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Abstract")

#-------------------------------------------------------------------------------
# Variable Definitions
#-------------------------------------------------------------------------------

set(PNAME_LOWER "abstract")
set(PNAME_CAPITALIZED "Abstract")

#-------------------------------------------------------------------------------
# Library Definition
#-------------------------------------------------------------------------------

add_library("${PNAME_LOWER}" INTERFACE)
add_library("${PNAME_CAPITALIZED}::${PNAME_LOWER}" ALIAS "${PNAME_LOWER}")

target_sources("${PNAME_LOWER}"
  INTERFACE
    "view.hpp"
)

target_include_directories("${PNAME_LOWER}"
  INTERFACE
    "${CMAKE_SOURCE_DIR}/src"
)
