#-------------------------------------------------------------------------------
# Variable definitions
#-------------------------------------------------------------------------------

set(PROJECT_NAME_CAP "Presenter")
set(PROJECT_NAME_LOW "presenter")

#-------------------------------------------------------------------------------
# Project definition
#-------------------------------------------------------------------------------

project("${PROJECT_NAME_CAP}")

#-------------------------------------------------------------------------------
# Library definition
#-------------------------------------------------------------------------------

add_library("${PROJECT_NAME_LOW}" INTERFACE)
add_library("${PROJECT_NAME_CAP}::${PROJECT_NAME_LOW}" ALIAS "${PROJECT_NAME_LOW}")

target_sources("${PROJECT_NAME_LOW}"
  INTERFACE
    "input_presenter.hpp"
    "output_presenter.hpp"
)

target_include_directories("${PROJECT_NAME_LOW}"
  INTERFACE
    "${CMAKE_SOURCE_DIR}/src"
)

#-------------------------------------------------------------------------------
# Link libraries
#-------------------------------------------------------------------------------

target_link_libraries("${PROJECT_NAME_LOW}"
  INTERFACE
    "${LIB_ABSTRACT}"
    "${LIB_MODEL}"
)
