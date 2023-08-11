# The name of the file to store the raw coverage data
set(COVERAGE_INFO_FILE "coverage.info")

# The name of the file to store the filtered coverage data
set(FILTERED_COVERAGE_INFO "filtered_coverage.info")

# Generates a coverage target for the specified target using LCOV and genhtml.
#
# @param target The name of the target to generate a coverage target for.
function(generate_coverage_target target)
  # Find the LCOV and genhtml programs
  find_program(LCOV_PATH "lcov" REQUIRED)
  find_program(GENHTML_PATH "genhtml" REQUIRED)

  # Add a coverage target
  add_custom_target("coverage"
    # Build this target by default
    ALL

    # Add a dependency on the specified target
    DEPENDS
      "${target}"

    # Set the working directory to the binary directory
    WORKING_DIRECTORY
      ${CMAKE_BINARY_DIR}

    # Comment for the coverage target
    COMMENT
      "Generating test coverage report for the ${target} target..."

    # Remove any existing coverage files
    COMMAND
      rm -rf ${COVERAGE_INFO_FILE} ${FILTERED_COVERAGE_INFO}

    # Reset the coverage counters
    COMMAND
      ${LCOV_PATH} --directory . --zerocounters

    # Run the specified target
    COMMAND
      $<TARGET_FILE:${target}>

    # Capture the coverage data
    COMMAND
      ${LCOV_PATH} --capture --directory . --output-file ${COVERAGE_INFO_FILE}

    # Filter out files
    COMMAND
      ${LCOV_PATH} --remove ${COVERAGE_INFO_FILE} '/usr/*' --output-file ${FILTERED_COVERAGE_INFO}

    # Generate an HTML report using genhtml
    COMMAND
      ${GENHTML_PATH} --output-directory coverage ${FILTERED_COVERAGE_INFO} --legend
  )
endfunction()
