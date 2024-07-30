#-------------------------------------------------------------------------------
# Variable definitions
#-------------------------------------------------------------------------------

set(PROJECT_NAME_CAP "Abstract")
set(PROJECT_NAME_LOW "abstract")

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
    "view.hpp"
)

target_include_directories("${PROJECT_NAME_LOW}"
  INTERFACE
    "${CMAKE_SOURCE_DIR}/src"
)
