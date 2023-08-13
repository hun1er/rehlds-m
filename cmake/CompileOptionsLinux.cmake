#-------------------------------------------------------------------------------
# Compile Definitions
#-------------------------------------------------------------------------------

# Adds additional compile definitions for a list of targets.
#
# @param TARGETS The list of target names to add compile definitions for.
function(configure_compile_definitions)
  # Parse the arguments passed to the function
  cmake_parse_arguments("CD" "" "" "TARGETS" ${ARGN})

  # If TARGETS is not set, print an error message and stop processing
  if(NOT CD_TARGETS)
    message(FATAL_ERROR "TARGETS is not set.")
  endif()

  # Loop over the list of targets, add compile definitions
  foreach(target_name IN LISTS CD_TARGETS)
    message(STATUS "Configuring compile definitions for target \"${target_name}\"")
  endforeach()
endfunction()

#-------------------------------------------------------------------------------
# Compile Options
#-------------------------------------------------------------------------------

set(CMAKE_C_FLAGS_DEBUG
  "-g -fno-omit-frame-pointer"
)

set(CMAKE_C_FLAGS_RELEASE
  "-O3 -fno-stack-protector -fomit-frame-pointer -DNDEBUG"
)

set(CMAKE_C_FLAGS_MINSIZEREL
  "-Os -fno-stack-protector -fomit-frame-pointer -DNDEBUG"
)

set(CMAKE_C_FLAGS_RELWITHDEBINFO
  "-O2 -g -DNDEBUG"
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")

add_compile_options(
  -m32                # Generate code for 32-bit environment
  -mmmx               # Enable MMX extended instruction set
  -msse               # Enable SSE extended instruction set
  -msse2              # Enable SSE2 extended instruction set
  -msse3              # Enable SSE3 extended instruction set
  -mssse3             # Enable SSSE3 extended instruction set
  -msse4              # Enable SSE4 extended instruction set
  -msse4.1            # Enable SSE4.1 extended instruction set
  -msse4.2            # Enable SSE4.2 extended instruction set
  -mfpmath=sse        # Use scalar floating-point instructions present in the SSE instruction set
  -pipe               # Use pipes rather than intermediate files
  -fdata-sections     # Place each data in its own section
  -ffunction-sections # Place each function in its own section

  # Generate instructions for a specified machine type
  $<IF:$<BOOL:${OPTIMIZE_FOR_CURRENT_CPU}>,-march=native,-march=x86-64-v2>

  # Tune to the specified cpu-type everything applicable about the generated code
  $<$<NOT:$<BOOL:${OPTIMIZE_FOR_CURRENT_CPU}>>:-mtune=generic>

  # AddressSanitizer
  $<$<BOOL:${ENABLE_ASAN}>:-fsanitize=address>

  # UndefinedBehaviorSanitizer
  $<$<BOOL:${ENABLE_UBSAN}>:-fsanitize=undefined>

  # Enable/Disable RTTI support
  $<$<COMPILE_LANGUAGE:CXX>:$<IF:$<BOOL:${ENABLE_RTTI}>,-frtti,-fno-rtti>>

  # Enable/Disable C++ exception handling
  $<$<COMPILE_LANGUAGE:CXX>:$<IF:$<BOOL:${ENABLE_EXCEPTIONS}>,-fexceptions,-fno-exceptions>>
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
  endforeach()
endfunction()

#-------------------------------------------------------------------------------
# Link Options
#-------------------------------------------------------------------------------

add_link_options(
  -m32                # Generate code for 32-bit environment
  -Wl,--as-needed     # Only link libraries as needed
  -Wl,--gc-sections   # Perform garbage collection of unused input sections
  -Wl,--no-undefined  # Do not allow undefined symbols

  # Generate instructions for a specified machine type
  $<IF:$<BOOL:${OPTIMIZE_FOR_CURRENT_CPU}>,-march=native,-march=x86-64-v2>

  # Tune to the specified cpu-type everything applicable about the generated code
  $<$<NOT:$<BOOL:${OPTIMIZE_FOR_CURRENT_CPU}>>:-mtune=generic>

  # Warn about common symbols when AddressSanitizer is not enabled
  $<$<NOT:$<BOOL:${ENABLE_ASAN}>>:-Wl,--warn-common>

  # Compress debug sections using zlib
  $<$<CONFIG:RelWithDebInfo>:-Wl,--compress-debug-sections=zlib>

  # Strip all symbols
  $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:-Wl,--strip-all>

  # Discard all local symbols
  $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>,$<CONFIG:RelWithDebInfo>>:-Wl,--discard-all>

  # AddressSanitizer
  $<$<BOOL:${ENABLE_ASAN}>:-fsanitize=address>

  # UndefinedBehaviorSanitizer
  $<$<BOOL:${ENABLE_UBSAN}>:-fsanitize=undefined>
)

# Check if the project is not using the Gold or LLD linker
if(NOT USE_LINKER_GOLD AND NOT USE_LINKER_LLD)
  add_link_options(
    -Wl,-O3                         # Optimize for speed
    -Wl,--check-sections            # Check section addresses for overlaps
    -Wl,--no-allow-shlib-undefined  # Do not allow undefined symbols in shared libraries
    -Wl,--relax                     # Relax branch and call instructions
    -Wl,--warn-alternate-em         # Warn about alternate entry points
  )
endif()

# Adds additional link options for a list of targets.
#
# @param TARGETS The list of target names to add link options for.
function(configure_link_options)
  # Parse the arguments passed to the function
  cmake_parse_arguments("LO" "" "" "TARGETS" ${ARGN})

  # If TARGETS is not set, print an error message and stop processing
  if(NOT LO_TARGETS)
    message(FATAL_ERROR "TARGETS is not set.")
  endif()

  # Loop over the list of targets, add link options
  foreach(target_name IN LISTS LO_TARGETS)
    message(STATUS "Configuring link options for target \"${target_name}\"")
  endforeach()
endfunction()

#-------------------------------------------------------------------------------
# Compiler-Specific Options
#-------------------------------------------------------------------------------

include_compiler_options("${CMAKE_C_COMPILER_ID}" "C")
include_compiler_options("${CMAKE_CXX_COMPILER_ID}" "CXX")
