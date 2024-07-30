#-------------------------------------------------------------------------------
# Compile Options
#-------------------------------------------------------------------------------

add_compile_options(
  # Optimize for debugging for Debug builds
  $<$<CONFIG:Debug>:-Og>

  # Generate debugging information for gdb for Debug and RelWithDebInfo builds
  $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-ggdb>
)

# Check if the GCC compiler version is greater than or equal to 12
if(CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 12)
  set(CMAKE_C_COMPILE_OPTIONS_IPO
    -flto=auto            # Use Link Time Optimization (LTO) automatically
    -fno-fat-lto-objects  # Do not generate fat LTO objects
  )
endif()

# Check if the G++ compiler version is greater than or equal to 12
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 12)
  set(CMAKE_CXX_COMPILE_OPTIONS_IPO
    -flto=auto            # Use Link Time Optimization (LTO) automatically
    -fno-fat-lto-objects  # Do not generate fat LTO objects
  )
endif()

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
        # Common diagnostic flags
        -Wall
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wdouble-promotion
        -Wduplicated-branches
        -Wduplicated-cond
        -Wextra
        -Wfloat-equal
        -Wformat=2
        -Wlogical-op
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
          -Wuseless-cast
          -Wzero-as-null-pointer-constant
          #-Wno-non-virtual-dtor
        >
    )
  endforeach()
endfunction()

#-------------------------------------------------------------------------------
# Link Options
#-------------------------------------------------------------------------------

# Check if the project is using the Gold linker
if(USE_LINKER_GOLD)
  add_link_options(
    -fuse-ld=gold                       # Use the gold linker
    -Wl,-O3                             # Optimize for speed
    -Wl,--icf-iterations=5              # Perform 5 ICF iterations
    -Wl,--icf=safe                      # Perform Identical Code Folding (ICF) safely
    -Wl,--no-incremental                # Do not perform incremental linking
    -Wl,--no-whole-archive              # Do not include all archive members
    -Wl,--relax                         # Relax branch and call instructions
    -Wl,--unresolved-symbols=report-all # Report all unresolved symbols
    -Wl,--warn-drop-version             # Warn about dropped version information
    -Wl,--warn-execstack                # Warn about executable stacks
    -Wl,--warn-search-mismatch          # Warn about search path mismatches
    -Wl,--warn-shared-textrel           # Warn about shared text relocations

    #-Wl,--print-gc-sections            # Print garbage collected sections
    #-Wl,--print-icf-sections           # Print ICF sections
    #-Wl,--stats                        # Print linker statistics

    # Detect One Definition Rule (ODR) violations when AddressSanitizer is not enabled
    $<$<NOT:$<BOOL:${ENABLE_ASAN}>>:-Wl,--detect-odr-violations>

    # Do not generate unwind information for Release or MinSizeRel builds
    $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:-Wl,--no-ld-generated-unwind-info>
  )
endif()
