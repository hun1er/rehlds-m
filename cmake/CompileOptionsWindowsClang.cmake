#-------------------------------------------------------------------------------
# Compile Options
#-------------------------------------------------------------------------------

add_compile_options(
  -mmmx                 # Enable MMX extended instruction set
  -msse                 # Enable SSE extended instruction set
  -msse2                # Enable SSE2 extended instruction set
  -msse3                # Enable SSE3 extended instruction set
  -mssse3               # Enable SSSE3 extended instruction set
  -msse4.1              # Enable SSE4.1 extended instruction set
  -msse4.2              # Enable SSE4.2 extended instruction set
  -fcf-protection=none  # Instrument control-flow architecture protection
  /Qvec                 # Loop vectorization passes

  # Generate instructions for a specified machine type
  $<IF:$<BOOL:${OPTIMIZE_FOR_CURRENT_CPU}>,-march=native,-march=x86-64-v2>
)

# Adds additional compile options for a list of targets.
#
# @param TARGETS The list of target names to add compile options for.
function(configure_compile_options)
  # Parse the arguments passed to the function
  cmake_parse_arguments("CO" "" "" "TARGETS" ${ARGN})

  # If TARGETS is not set, print an error message and stop processing
  if(NOT CO_TARGETS)
    message(FATAL_ERROR "TARGETS is not set.")
  endif()

  # Loop over the list of targets, add compile options
  foreach(target_name IN LISTS CO_TARGETS)
    message(STATUS "Configuring compile options for target \"${target_name}\"")

    target_compile_options("${target_name}"
      PRIVATE
        /W4             # Output warning level
        /permissive-    # Standard-conformance mode
        /volatile:iso   # volatile keyword interpretation
        /options:strict # Unrecognized compiler options are errors

        # Common diagnostic flags
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wdouble-promotion
        -Wextra
        -Wfloat-equal
        -Wformat=2
        -Wnull-dereference
        -Wpedantic
        -Wredundant-decls
        -Wshadow
        -Wundef
        -Wunused
        -Wwrite-strings

        # C++ diagnostic flags
        $<$<COMPILE_LANGUAGE:CXX>:
          -Wctor-dtor-privacy
          -Weffc++
          -Wextra-semi
          -Wnon-virtual-dtor
          -Wold-style-cast
          -Woverloaded-virtual
          -Wsign-promo
          -Wzero-as-null-pointer-constant
        >

        # Warning suppression
        -Wno-sign-conversion

        # Buffer security check
        $<IF:$<CONFIG:Debug>,/GS,/GS->

        # Additional security checks
        $<IF:$<CONFIG:Debug>,/sdl,/sdl->
    )
  endforeach()
endfunction()
