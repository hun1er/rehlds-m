#-------------------------------------------------------------------------------
# Compiler Flags
#-------------------------------------------------------------------------------

# Set the compiler options for interprocedural optimization (IPO)
if("${CMAKE_C_COMPILER_ID}" STREQUAL "IntelLLVM")
  set(CMAKE_C_COMPILE_OPTIONS_IPO "-flto=full")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "IntelLLVM")
  set(CMAKE_CXX_COMPILE_OPTIONS_IPO "-flto=full")
endif()

add_compile_options(
  # Optimize for debugging for Debug builds
  $<$<CONFIG:Debug>:-Og>

  # Generate debugging information for gdb for Debug and RelWithDebInfo builds
  $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-ggdb>
)

#-------------------------------------------------------------------------------
# Configure Functions
#-------------------------------------------------------------------------------

# Adds additional compile options for a list of targets.
#
# @param TARGETS The list of target names to add compile options for.
function(configure_compile_options)
  # Parse the arguments passed to the function
  cmake_parse_arguments("ARG" "" "" "TARGETS" ${ARGN})

  # If TARGETS is not set, print an error message and stop processing
  if(NOT ARG_TARGETS)
    message(FATAL_ERROR "TARGETS argument is required.")
  endif()

  # Loop over the list of targets, add compile options
  foreach(target_name IN LISTS ARG_TARGETS)
    message(STATUS "Configuring compile options for target \"${target_name}\"")
    target_compile_options("${target_name}"
      PRIVATE
        # Common diagnostic flags
        -Wall
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wdeprecated
        -Wdouble-promotion
        -Wextra
        -Wfloat-equal
        -Wformat=2
        -Wnull-dereference
        -Wpedantic
        -Wredundant-decls
        -Wshadow
        -Wsign-conversion
        -Wundef
        -Wunused
        -Wwrite-strings
        #-Wno-sign-conversion

        # C++ diagnostic flags
        $<$<COMPILE_LANGUAGE:CXX>:
          -Wctor-dtor-privacy
          -Wextra-semi
          -Wnon-virtual-dtor
          -Wold-style-cast
          -Woverloaded-virtual
          -Wsign-promo
          -Wzero-as-null-pointer-constant
          #-Wno-non-virtual-dtor
        >

        # Use the Short Vector Math Library (SVML)
        $<$<BOOL:${USE_INTEL_SVML}>:-fimf-use-svml=true>

        -fcolor-diagnostics       # Enable colors in diagnostics
        -fdiagnostics-show-option # Print option name with mappable diagnostics
        -pedantic                 # Warn on language extensions

        $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>,$<CONFIG:RelWithDebInfo>>:
          -finline-functions      # Inline suitable functions
          -freroll-loops          # Turn on loop reroller
          -fvectorize             # Enable the loop vectorization passes
        >
    )

    target_link_libraries("${target_name}"
      PRIVATE
        # Restrict linking of all Intel specific libraries
        $<$<BOOL:${NO_INTEL_LIB}>:-no-intel-lib>

        # Link Intel provided libraries statically or dynamically
        $<IF:$<BOOL:${LINK_STATIC_INTEL}>,-static-intel,-shared-intel>
    )
  endforeach()
endfunction()
