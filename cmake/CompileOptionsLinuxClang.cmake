#-------------------------------------------------------------------------------
# Compiler Flags
#-------------------------------------------------------------------------------

# Set the compiler options for interprocedural optimization (IPO)
if("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_C_COMPILE_OPTIONS_IPO "-flto=full")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_COMPILE_OPTIONS_IPO "-flto=full")
endif()

add_compile_options(
  # Optimize for debugging for Debug builds
  $<$<CONFIG:Debug>:-Og>

  # Generate debugging information for gdb for Debug and RelWithDebInfo builds
  $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-ggdb>
)

#-------------------------------------------------------------------------------
# Linker Flags
#-------------------------------------------------------------------------------

# Check if the project is using the LLD linker
if(USE_LINKER_LLD)
  add_link_options(
    -fuse-ld=lld                      # Use LLD linker
    -Wl,-O2                           # Optimize for speed
    -Wl,--check-sections              # Check section addresses for overlaps
    -Wl,--icf=safe                    # Perform Identical Code Folding (ICF) safely
    -Wl,--warn-backrefs               # Warn about backreferences in linker scripts
    -Wl,--warn-ifunc-textrel          # Warn about text relocations in ifunc symbols
    -Wl,--warn-symbol-ordering        # Warn about out-of-order symbols

    #-Wl,--print-gc-sections          # Print removed unused sections
    #-Wl,--print-icf-sections         # Print folded identical sections

    # Use LTO optimization level 3 for Release and MinSizeRel builds
    $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:-Wl,--lto-O3>
  )
endif()

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
          -Weffc++
          -Wextra-semi
          -Wnon-virtual-dtor
          -Wold-style-cast
          -Woverloaded-virtual
          -Wsign-promo
          -Wzero-as-null-pointer-constant
          #-Wno-non-virtual-dtor
        >
    )
  endforeach()
endfunction()
