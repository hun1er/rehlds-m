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
  cmake_parse_arguments("PVS" ""
    "PLATFORM;PREPROCESSOR;CONFIG;SUPPRESS_BASE"
    "TARGETS;MODE;DEPENDS;C_FLAGS;CXX_FLAGS;ARGS;CONVERTER_ARGS"
    ${ARGN}
  )

  # If TARGETS is not set, print an error message and stop processing
  if(NOT PVS_TARGETS)
    message(FATAL_ERROR "TARGETS is not set.")
  endif()

  # Set default value for MODE if not defined
  if(NOT DEFINED PVS_MODE)
    list(APPEND PVS_MODE
      #AUTOSAR:1,2,3
      #MISRA:1,2,3
      OWASP:1,2,3
      GA:1,2,3
      OP:1,2,3
      #64:1,2,3
    )
  endif()

  # Set value of PLATFORM based on operating system
  if(NOT DEFINED PVS_PLATFORM)
    if(APPLE)
      set(PVS_PLATFORM "macos")
    elseif(UNIX)
      set(PVS_PLATFORM "linux32")
    else()
      set(PVS_PLATFORM "win32")
    endif()
  endif()

  # Set value of PREPROCESSOR based on compiler being used
  if(NOT DEFINED PVS_PREPROCESSOR)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
      set(PVS_PREPROCESSOR "clang")
    elseif(MSVC)
      set(PVS_PREPROCESSOR "visualcpp")
    else()
      set(PVS_PREPROCESSOR "gcc")
    endif()
  endif()

  # Set default value for CONFIG if not defined
  if(NOT DEFINED PVS_CONFIG)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.pvs-studio")
      set(PVS_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/.pvs-studio")
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/.pvs-studio")
      set(PVS_CONFIG "${CMAKE_SOURCE_DIR}/.pvs-studio")
    endif()
  endif()

  # Set default value for SUPPRESS_BASE if not defined
  if(NOT DEFINED PVS_SUPPRESS_BASE)
    set(PVS_SUPPRESS_BASE "")
  endif()

  # Set default value for DEPENDS if not defined
  if(NOT DEFINED PVS_DEPENDS)
    set(PVS_DEPENDS "")
  endif()

  # Set default value for C_FLAGS if not defined
  if(NOT DEFINED PVS_C_FLAGS)
    set(PVS_C_FLAGS "")
  endif()

  # Set default value for CXX_FLAGS if not defined
  if(NOT DEFINED PVS_CXX_FLAGS)
    set(PVS_CXX_FLAGS "")
  endif()

  # Set default value for ARGS if not defined
  list(APPEND PVS_ARGS "--threads" "${PROCESSOR_CORES}")

  # Set default value for CONVERTER_ARGS if not defined
  if(NOT DEFINED PVS_CONVERTER_ARGS)
    set(PVS_CONVERTER_ARGS --excludedCodes V1042)
  endif()

  # Include PVS-Studio.cmake file
  FetchContent_MakeAvailable(PVS_CMakeModule)
  include("${pvs_cmakemodule_SOURCE_DIR}/PVS-Studio.cmake")

  # Loop over the list of targets
  foreach(target_name IN LISTS PVS_TARGETS)
    message(STATUS "Enabling PVS-Studio for target \"${target_name}\"")
  endforeach()

  pvs_studio_add_target(
    # Target options:
    ALL                                   # Add PVS-Studio target to default build
    TARGET "pvs_analysis"                 # Name of analysis target
    ANALYZE ${PVS_TARGETS}                # Targets to analyze
    #RECURSIVE                            # Analyze target's dependencies (requires CMake 3.5+)
    #COMPILE_COMMANDS                     # Use compile_commands.json instead of targets (specified by the 'ANALYZE' option)

    # Output options:
    OUTPUT                                # Prints report to stdout
    #LOG path                             # Path to report (default: ${CMAKE_CURRENT_BINARY_DIR}/PVS-Studio.log)
    #FORMAT format                        # Format of report
    MODE ${PVS_MODE}                      # Analyzers/levels filter (default: GA:1,2)
    HIDE_HELP                             # Do not print help message

    # Analyzer options:
    PLATFORM "${PVS_PLATFORM}"            # linux32/linux64 (default: linux64)
    PREPROCESSOR "${PVS_PREPROCESSOR}"    # Preprocessor type: gcc/clang (default: auto detected)
    #LICENSE path                         # Path to PVS-Studio.lic (default: ~/.config/PVS-Studio/PVS-Studio.lic)
    CONFIG "${PVS_CONFIG}"                # Path to PVS-Studio.cfg
    #CFG_TEXT text                        # Embedded PVS-Studio.cfg
    SUPPRESS_BASE "${PVS_SUPPRESS_BASE}"  # Path to suppress base file
    #KEEP_COMBINED_PLOG                   # Do not delete combined plog file *.pvs.raw for further processing with plog-converter

    # Misc options:
    DEPENDS ${PVS_DEPENDS}                # Additional target dependencies
    #SOURCES path...                      # List of source files to analyze
    #BIN path                             # Path to pvs-studio-analyzer (Unix) or CompilerCommandsAnalyzer.exe (Windows)
    #CONVERTER path                       # Path to plog-converter (Unix) or HtmlGenerator.exe (Windows)
    C_FLAGS ${PVS_C_FLAGS}                # Additional C_FLAGS
    CXX_FLAGS ${PVS_CXX_FLAGS}            # Additional CXX_FLAGS
    ARGS ${PVS_ARGS}                      # Additional pvs-studio-analyzer/CompilerCommandsAnalyzer.exe flags
    CONVERTER_ARGS ${PVS_CONVERTER_ARGS}  # Additional plog-converter/HtmlGenerator.exe flags
  )
endfunction()
