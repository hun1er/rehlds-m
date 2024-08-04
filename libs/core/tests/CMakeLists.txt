#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("HLDS Core Unit Tests")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TESTED_TARGET_NAME "core")
set(TARGET_NAME "${TESTED_TARGET_NAME}_tests")
add_executable("${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

set(HPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/include/${TESTED_TARGET_NAME}")
set(CPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/src/${TESTED_TARGET_NAME}")

target_sources("${TARGET_NAME}"
  PRIVATE
    "${CPP_SOURCES_DIR}/cmdline_args.cpp"
)

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${TARGET_NAME}"
  PRIVATE
    "${LIB_CORE}"
    "${LIB_GTEST}"
)

#-------------------------------------------------------------------------------
# Test Configuration
#-------------------------------------------------------------------------------

gtest_discover_tests("${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Code Coverage
#-------------------------------------------------------------------------------

if(CODE_COVERAGE)
  generate_coverage_target("${TARGET_NAME}")
endif()
