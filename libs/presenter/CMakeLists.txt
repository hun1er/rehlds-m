#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Presenter")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "presenter")
set(TARGET_ALIAS "Presenter::${TARGET_NAME}")

add_library("${TARGET_NAME}" INTERFACE)
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

set(HPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/include/${TARGET_NAME}")
set(CPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/src/${TARGET_NAME}")

target_sources("${TARGET_NAME}"
  INTERFACE
    "${HPP_SOURCES_DIR}/input_presenter.hpp"
    "${HPP_SOURCES_DIR}/output_presenter.hpp"
)

#-------------------------------------------------------------------------------
# Include Directories
#-------------------------------------------------------------------------------

target_include_directories("${TARGET_NAME}"
  INTERFACE
    "${PROJECT_SOURCE_DIR}/include"
)

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${TARGET_NAME}"
  INTERFACE
    "${LIB_ABSTRACT}"
    "${LIB_MODEL}"
)
