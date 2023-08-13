cmake_minimum_required(VERSION 3.21...3.27)

#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Presenter")

#-------------------------------------------------------------------------------
# Variable Definitions
#-------------------------------------------------------------------------------

set(PNAME_LOWER "presenter")
set(PNAME_CAPITALIZED "Presenter")

#-------------------------------------------------------------------------------
# Library Definition
#-------------------------------------------------------------------------------

add_library("${PNAME_LOWER}" INTERFACE)
add_library("${PNAME_CAPITALIZED}::${PNAME_LOWER}" ALIAS "${PNAME_LOWER}")

target_sources("${PNAME_LOWER}"
  INTERFACE
    "input_presenter.hpp"
    "output_presenter.hpp"
)

target_include_directories("${PNAME_LOWER}"
  INTERFACE
    "${CMAKE_SOURCE_DIR}/src"
)

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${PNAME_LOWER}"
  INTERFACE
    "Abstract::abstract"
    "Model::model"
)
