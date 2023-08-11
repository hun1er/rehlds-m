# Enables Cppcheck for a list of targets with the specified options.
#
# @param TARGETS The list of target names to enable Cppcheck for.
# @param PLATFORM [optional] The platform to use for Cppcheck.
# @param ENABLE [optional] The checks to enable in Cppcheck.
# @param DISABLE [optional] The checks to disable in Cppcheck.
# @param LIBRARY [optional] The libraries to use in Cppcheck.
# @param INCONCLUSIVE [optional] Enable inconclusive checks in Cppcheck.
# @param CTU_DEPTH [optional] The maximum depth for whole program analysis in Cppcheck.
# @param QUIET [optional] Enable quiet mode in Cppcheck.
# @param VERBOSE [optional] Enable verbose mode in Cppcheck.
# @param CHECK_CONFIG [optional] Check the configuration in Cppcheck.
function(enable_cppcheck)
  # Parse the arguments passed to the function
  cmake_parse_arguments("CPPC" ""
    "PLATFORM;ENABLE;DISABLE;LIBRARY;INCONCLUSIVE;CTU_DEPTH;QUIET;VERBOSE;CHECK_CONFIG"
    "TARGETS"
    ${ARGN}
  )

  # If TARGETS is not set, print an error message and stop processing
  if(NOT CPPC_TARGETS)
    message(FATAL_ERROR "TARGETS is not set.")
  endif()

  # If PLATFORM is not set, set it to "native"
  if(NOT DEFINED CPPC_PLATFORM)
    if(WIN32)
      set(CPPC_PLATFORM "win32A")
    else()
      set(CPPC_PLATFORM "unix32")
    endif()
  endif()

  # If ENABLE is not set, set it to "all"
  if(NOT DEFINED CPPC_ENABLE)
    set(CPPC_ENABLE "all")
  endif()

  # If DISABLE is not set, set it to "unusedFunction"
  if(NOT DEFINED CPPC_DISABLE)
    set(CPPC_DISABLE "unusedFunction")
  endif()

  # If LIBRARY is not set, set it to a list of libraries
  if(NOT DEFINED CPPC_LIBRARY)
    set(CPPC_LIBRARY "boost,gnu,googletest,libcurl,posix,qt,sdl,std,windows,zlib")
  endif()

  # If CTU_DEPTH is not set, set it to "16"
  if(NOT DEFINED CPPC_CTU_DEPTH)
    set(CPPC_CTU_DEPTH "16")
  endif()

  # If QUIET is not set, set it to ON
  if(NOT DEFINED CPPC_QUIET)
    set(CPPC_QUIET ON)
  endif()

  # Find the cppcheck executable
  find_program(cppcheck_exe
    NAMES "cppcheck"
    HINTS "$ENV{PROGRAMFILES}/Cppcheck"
  )

  # If cppcheck is not found, print a warning and return
  if(NOT cppcheck_exe)
    message(WARNING "Cppcheck not found")
    return()
  endif()

  # Add options to the cppcheck command
  list(APPEND cppcheck_exe
    "-j${PROCESSOR_CORES}" # Start <jobs> threads to do the checking simultaneously.
    "--platform=${CPPC_PLATFORM}" # Specifies platform specific types and sizes.
    "--enable=${CPPC_ENABLE}" # Enable additional checks.
    "--disable=${CPPC_DISABLE}" # Disable individual checks.
    "--library=${CPPC_LIBRARY}" # Load file <cfg> that contains information about types and functions.
    "--max-configs=50" # Maximum number of configurations to check in a file before skipping it.
    "--max-ctu-depth=${CPPC_CTU_DEPTH}" # Max depth in whole program analysis.
    "--inline-suppr" # Enable inline suppressions.
  )

  # If the suppression file exists, add it to the cppcheck command
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.cppcheck_suppressions")
    list(APPEND cppcheck_exe "--suppressions-list=${CMAKE_CURRENT_SOURCE_DIR}/.cppcheck_suppressions")
  elseif(EXISTS "${CMAKE_SOURCE_DIR}/.cppcheck_suppressions")
    list(APPEND cppcheck_exe "--suppressions-list=${CMAKE_SOURCE_DIR}/.cppcheck_suppressions")
  endif()

  # Set the source directory
  set(src_dir "${PROJECT_SOURCE_DIR}/src")

  # If the source directory exists, add it to the include path for cppcheck
  if(EXISTS "${src_dir}")
    list(APPEND cppcheck_exe "-I${src_dir}")
  endif()

  # If the INCONCLUSIVE option is specified, add it to the cppcheck command
  if(CPPC_INCONCLUSIVE)
    # Allow that Cppcheck reports even though the analysis is inconclusive.
    list(APPEND cppcheck_exe "--inconclusive")
  endif()

  # If the QUIET option is specified, add it to the cppcheck command
  if(CPPC_QUIET)
    # Do not show progress reports.
    list(APPEND cppcheck_exe "--quiet")
  endif()

  # If VERBOSE is specified, add it to the cppcheck command
  if(VERBOSE)
    # Output more detailed error information.
    list(APPEND cppcheck_exe "--verbose")
  endif()

  # If CHECK_CONFIG is specified, add it to the cppcheck command
  if(CPPC_CHECK_CONFIG)
    # Check cppcheck configuration.
    list(APPEND cppcheck_exe "--check-config")
  endif()

  # Loop over the list of targets, enable cppcheck for that target
  foreach(target_name IN LISTS CPPC_TARGETS)
    message(STATUS "Enabling Cppcheck for target \"${target_name}\"")

    # Set the working directory for cppcheck and create it if it doesn't exist
    set(cppcheck_build_dir "${PROJECT_BINARY_DIR}/Cppcheck/${target_name}")
    file(MAKE_DIRECTORY "${cppcheck_build_dir}")

    # Add the working directory to the cppcheck command for this target
    set(cppcheck_exe_target "${cppcheck_exe}")
    list(APPEND cppcheck_exe_target "--cppcheck-build-dir=${cppcheck_build_dir}")

    # Get the include directories for this target
    get_property(target_includes TARGET "${target_name}" PROPERTY INCLUDE_DIRECTORIES)

    # If include directories are found, add them to the cppcheck command for this target
    if(target_includes)
      foreach(inc_dir IN LISTS target_includes)
        if(EXISTS "${inc_dir}")
          list(APPEND cppcheck_exe_target "-I${inc_dir}")
        endif()
      endforeach()
    endif()

    # Set the C standards for this target's cppcheck command
    set(cppcheck_exe_target_c "${cppcheck_exe_target}")
    list(APPEND cppcheck_exe_target_c "--std=c${CMAKE_C_STANDARD}")

    # Set the C++ standards for this target's cppcheck command
    set(cppcheck_exe_target_cpp "${cppcheck_exe_target}")
    list(APPEND cppcheck_exe_target_cpp "--std=c++${CMAKE_CXX_STANDARD}")

    # Set the C_CPPCHECK and CXX_CPPCHECK properties for this target
    # to enable cppckeck for this target's C and C++ files respectively.
    set_target_properties("${target_name}" PROPERTIES
      C_CPPCHECK "${cppcheck_exe_target_c}"
      CXX_CPPCHECK "${cppcheck_exe_target_cpp}"
    )
  endforeach()
endfunction()
