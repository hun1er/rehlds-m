# Enables Include What You Use (IWYU) for a list of targets with the specified options.
#
# @param TARGETS The list of target names to enable IWYU for.
# @param MAPPING [optional] The mapping files to use for IWYU.
# @param TRANSITIVE_INCLUDES_ONLY [optional] If set, only include transitive includes in IWYU analysis.
# @param MAX_LINE_LENGTH [optional] The maximum line length to use for IWYU.
# @param COMMENT_STYLE [optional] The comment style to use for IWYU.
# @param UPDATE_COMMENTS [optional] If set, update comments in IWYU analysis.
# @param NO_COMMENTS [optional] If set, do not include comments in IWYU analysis.
# @param VERBOSE [optional] The verbosity level to use for IWYU.
function(enable_iwyu)
  # Parse the arguments passed to the function
  cmake_parse_arguments("IWYU" ""
    "TRANSITIVE_INCLUDES_ONLY;MAX_LINE_LENGTH;COMMENT_STYLE;UPDATE_COMMENTS;NO_COMMENTS;VERBOSE"
    "TARGETS;MAPPING"
    ${ARGN}
  )

  # Find the IWYU executable
  find_program(iwyu_exe
    NAMES "include-what-you-use" "iwyu"
    HINTS "$ENV{PROGRAMFILES}/Include What You Use" "$ENV{PROGRAMFILES}/IWYU"
  )

  # If iwyu is not found, print a warning and return
  if(NOT iwyu_exe)
    message(WARNING "Include what you use not found")
    return()
  endif()

  # If a mapping file is defined, add it to the IWYU command line arguments
  if(DEFINED IWYU_MAPPING)
    foreach(mapping_file IN LISTS IWYU_MAPPING)
      list(APPEND iwyu_exe "-Xiwyu;--mapping_file=${mapping_file}")
    endforeach()
  endif()

  # If TRANSITIVE_INCLUDES_ONLY is set, add it to the IWYU command line arguments
  if(IWYU_TRANSITIVE_INCLUDES_ONLY)
    list(APPEND iwyu_exe "-Xiwyu;--transitive_includes_only")
  endif()

  # If COMMENT_STYLE is defined, add it to the IWYU command line arguments
  if(DEFINED IWYU_COMMENT_STYLE)
    list(APPEND iwyu_exe "-Xiwyu;--comment_style=${IWYU_COMMENT_STYLE}")
  endif()

  # If UPDATE_COMMENTS is not defined or is set to true, add it to the IWYU command line arguments
  if(NOT DEFINED IWYU_UPDATE_COMMENTS OR IWYU_UPDATE_COMMENTS)
    list(APPEND iwyu_exe "-Xiwyu;--update_comments")
  endif()

  # If NO_COMMENTS is not defined or is set to true, add it to the IWYU command line arguments
  if(NOT DEFINED IWYU_NO_COMMENTS OR IWYU_NO_COMMENTS)
    list(APPEND iwyu_exe "-Xiwyu;--no_comments")
  endif()

  # If VERBOSE is defined, add it to the IWYU command line arguments
  if(DEFINED IWYU_VERBOSE)
    list(APPEND iwyu_exe "-Xiwyu;--verbose=${IWYU_VERBOSE}")
  endif()

  # If MAX_LINE_LENGTH is not defined, set it to a default value of "120"
  if(NOT DEFINED IWYU_MAX_LINE_LENGTH)
    set(IWYU_MAX_LINE_LENGTH "120")
  endif()

  # If C++ standard is greater than or equal to C++17, add it to the IWYU command line arguments
  if(${CMAKE_CXX_STANDARD} GREATER_EQUAL 17)
    list(APPEND iwyu_exe "-Xiwyu;--cxx17ns")
  endif()

  # Add additional options to the IWYU command line arguments
  list(APPEND iwyu_exe
    "-Xiwyu;--quoted_includes_first"
    "-Xiwyu;--max_line_length=${IWYU_MAX_LINE_LENGTH}"
    "-Wno-unknown-warning-option"
    "-Wno-ignored-optimization-argument"
  )

  # If TARGETS is not set, print an error message and stop processing
  if(NOT IWYU_TARGETS)
    message(FATAL_ERROR "TARGETS is not set.")
  endif()

  # Loop over the list of targets, enable cppcheck for that target
  foreach(target_name IN LISTS IWYU_TARGETS)
    message(STATUS "Enabling Include What You Use for target \"${target_name}\"")

    set_target_properties("${target_name}" PROPERTIES
      C_INCLUDE_WHAT_YOU_USE "${iwyu_exe}"
      CXX_INCLUDE_WHAT_YOU_USE "${iwyu_exe}"
    )
  endforeach()
endfunction()
