# MODE defines the group of warnings that will be activated during analysis:
# 64 - (64-bit) - diagnostics that allow you to identify specific errors related to the development of 64-bit applications and migrating the code from a 32-bit platform to a 64-bit one;
# GA - (General Analysis) - general analysis rule set. The main set of PVS-Studio diagnostic rules;
# OP - (Optimization) - diagnostics of optimization. These are tips to improve the code efficiency;
# CS - (Customers' Specific) - highly specialized diagnostics, developed by user requests;
# MISRA - Set of diagnostics, developed according to the MISRA standard;
# OWASP - Set of diagnostics that allow you to find security issues and check code for compliance with OWASP ASVS standard;
# AUTOSAR - AUTOSAR guidelines.
# Several types of warnings must be separated by ';', e.g. 'GA;OP;64'.
#
# Levels:
# 1 corresponds to warnings of 'High' level;
# 2 to 'Medium' level warnings;
# 3 to 'Low' level warnings.

FetchContent_Declare(
  PVS_CMakeModule
  GIT_REPOSITORY  "https://github.com/viva64/pvs-studio-cmake-module.git"
  GIT_TAG         "master"
)

# Enables PVS-Studio for a list of targets with the specified options.
#
# @param TARGETS The list of target names to enable PVS-Studio for.
# @param PLATFORM [optional] The platform to use for PVS-Studio.
# @param PREPROCESSOR [optional] The preprocessor to use for PVS-Studio.
# @param CONFIG [optional] The configuration file to use for PVS-Studio.
# @param SUPPRESS_BASE [optional] The base suppressions file to use for PVS-Studio.
# @param MODE [optional] The analysis modes to use for PVS-Studio.
# @param DEPENDS [optional] The dependencies for the PVS-Studio analysis target.
# @param C_FLAGS [optional] The C compiler flags to use for PVS-Studio.
# @param CXX_FLAGS [optional] The C++ compiler flags to use for PVS-Studio.
# @param ARGS [optional] Additional arguments to pass to the PVS-Studio analyzer.
# @param CONVERTER_ARGS [optional] Additional arguments to pass to the PVS-Studio log converter.
function(enable_pvs_studio)
  # Parse the arguments passed to the function
  cmake_parse_arguments("ARG" ""
    "PLATFORM;PREPROCESSOR;CONFIG;SUPPRESS_BASE"
    "TARGETS;MODE;DEPENDS;C_FLAGS;CXX_FLAGS;ARGS;CONVERTER_ARGS"
    ${ARGN}
  )

  # If TARGETS is not set, print an error message and stop processing
  if(NOT ARG_TARGETS)
    message(FATAL_ERROR "TARGETS argument is required.")
  endif()

  # Set default value for MODE if not defined
  if(NOT DEFINED ARG_MODE)
    list(APPEND ARG_MODE
      #AUTOSAR:1,2,3
      #MISRA:1,2,3
      OWASP:1,2,3
      GA:1,2,3
      OP:1,2,3
      #64:1,2,3
    )
  endif()

  # Set value of PLATFORM based on operating system
  if(NOT DEFINED ARG_PLATFORM)
    if(APPLE)
      set(ARG_PLATFORM "macos")
    elseif(UNIX)
      set(ARG_PLATFORM "linux32")
    else()
      set(ARG_PLATFORM "win32")
    endif()
  endif()

  # Set value of PREPROCESSOR based on compiler being used
  if(NOT DEFINED ARG_PREPROCESSOR)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
      set(ARG_PREPROCESSOR "clang")
    elseif(MSVC)
      set(ARG_PREPROCESSOR "visualcpp")
    else()
      set(ARG_PREPROCESSOR "gcc")
    endif()
  endif()

  # Set default value for CONFIG if not defined
  if(NOT DEFINED ARG_CONFIG)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.pvs-studio")
      set(ARG_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/.pvs-studio")
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/.pvs-studio")
      set(ARG_CONFIG "${CMAKE_SOURCE_DIR}/.pvs-studio")
    endif()
  endif()

  # Set default value for SUPPRESS_BASE if not defined
  if(NOT DEFINED ARG_SUPPRESS_BASE)
    set(ARG_SUPPRESS_BASE "")
  endif()

  # Set default value for DEPENDS if not defined
  if(NOT DEFINED ARG_DEPENDS)
    set(ARG_DEPENDS "")
  endif()

  # Set default value for C_FLAGS if not defined
  if(NOT DEFINED ARG_C_FLAGS)
    set(ARG_C_FLAGS "")
  endif()

  # Set default value for CXX_FLAGS if not defined
  if(NOT DEFINED ARG_CXX_FLAGS)
    set(ARG_CXX_FLAGS "")
  endif()

  # Set default value for ARGS if not defined
  list(APPEND ARG_ARGS "--threads" "${PROCESSOR_CORES}")

  # Set default value for CONVERTER_ARGS if not defined
  if(NOT DEFINED ARG_CONVERTER_ARGS)
    set(ARG_CONVERTER_ARGS --excludedCodes V1042)
  endif()

  # Include PVS-Studio.cmake file
  FetchContent_MakeAvailable(PVS_CMakeModule)
  include("${pvs_cmakemodule_SOURCE_DIR}/PVS-Studio.cmake")

  # Loop over the list of targets
  foreach(target_name IN LISTS ARG_TARGETS)
    message(STATUS "Enabling PVS-Studio for target \"${target_name}\"")
  endforeach()

  pvs_studio_add_target(
    # Target options:
    ALL                                   # Add PVS-Studio target to default build
    TARGET "pvs_analysis"                 # Name of analysis target
    ANALYZE ${ARG_TARGETS}                # Targets to analyze
    #RECURSIVE                            # Analyze target's dependencies (requires CMake 3.5+)
    #COMPILE_COMMANDS                     # Use compile_commands.json instead of targets (specified by the 'ANALYZE' option)

    # Output options:
    OUTPUT                                # Prints report to stdout
    #LOG path                             # Path to report (default: ${CMAKE_CURRENT_BINARY_DIR}/PVS-Studio.log)
    #FORMAT format                        # Format of report
    MODE ${ARG_MODE}                      # Analyzers/levels filter (default: GA:1,2)
    HIDE_HELP                             # Do not print help message

    # Analyzer options:
    PLATFORM "${ARG_PLATFORM}"            # linux32/linux64 (default: linux64)
    PREPROCESSOR "${ARG_PREPROCESSOR}"    # Preprocessor type: gcc/clang (default: auto detected)
    #LICENSE path                         # Path to PVS-Studio.lic (default: ~/.config/PVS-Studio/PVS-Studio.lic)
    CONFIG "${ARG_CONFIG}"                # Path to PVS-Studio.cfg
    #CFG_TEXT text                        # Embedded PVS-Studio.cfg
    SUPPRESS_BASE "${ARG_SUPPRESS_BASE}"  # Path to suppress base file
    #KEEP_COMBINED_PLOG                   # Do not delete combined plog file *.pvs.raw for further processing with plog-converter

    # Misc options:
    DEPENDS ${ARG_DEPENDS}                # Additional target dependencies
    #SOURCES path...                      # List of source files to analyze
    #BIN path                             # Path to pvs-studio-analyzer (Unix) or CompilerCommandsAnalyzer.exe (Windows)
    #CONVERTER path                       # Path to plog-converter (Unix) or HtmlGenerator.exe (Windows)
    C_FLAGS ${ARG_C_FLAGS}                # Additional C_FLAGS
    CXX_FLAGS ${ARG_CXX_FLAGS}            # Additional CXX_FLAGS
    ARGS ${ARG_ARGS}                      # Additional pvs-studio-analyzer/CompilerCommandsAnalyzer.exe flags
    CONVERTER_ARGS ${ARG_CONVERTER_ARGS}  # Additional plog-converter/HtmlGenerator.exe flags
  )
endfunction()
