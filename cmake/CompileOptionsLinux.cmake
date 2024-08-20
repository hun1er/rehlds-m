#-------------------------------------------------------------------------------
# Compile Definitions
#-------------------------------------------------------------------------------

add_compile_definitions(
  $<$<CONFIG:Debug>:_FORTIFY_SOURCE=2>
  $<$<CONFIG:Debug>:_GLIBCXX_ASSERTIONS=1>
  $<$<CONFIG:Debug>:_GLIBCXX_CONCEPT_CHECKS=1>
  $<$<CONFIG:Debug>:_GLIBCXX_DEBUG_PEDANTIC=1>
  $<$<CONFIG:Debug>:_GLIBCXX_DEBUG=1>
)

#-------------------------------------------------------------------------------
# Compiler Flags
#-------------------------------------------------------------------------------

# Sets the appropriate -march compiler flag based on the compiler ID, version, and optimization preference.
#
# @param lang The programming language being used (C or CXX).
# @param compiler_id The ID of the compiler (GNU or Clang).
# @param compiler_version The version of the compiler.
function(set_march_flag lang compiler_id compiler_version)
  if(OPTIMIZE_FOR_CURRENT_CPU)
    set(march_flag "-march=native")
  elseif("${compiler_id}" STREQUAL "GNU" AND "${compiler_version}" VERSION_LESS "11")
    set(march_flag "-march=nehalem") # nehalem is close to x86-64-v2
  elseif("${compiler_id}" STREQUAL "Clang" AND "${compiler_version}" VERSION_LESS "12")
    set(march_flag "-march=nehalem") # nehalem is close to x86-64-v2
  else()
    set(march_flag "-march=x86-64-v2")
  endif()

  string(STRIP "${CMAKE_${lang}_FLAGS} ${march_flag}" march_flag)
  set(CMAKE_${lang}_FLAGS "${march_flag}" PARENT_SCOPE)
endfunction()

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
set_march_flag("C" "${CMAKE_C_COMPILER_ID}" "${CMAKE_C_COMPILER_VERSION}")
set_march_flag("CXX" "${CMAKE_CXX_COMPILER_ID}" "${CMAKE_CXX_COMPILER_VERSION}")

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

  # Use libc++ standard library
  $<$<BOOL:${LINK_LIBCPP}>:-stdlib=libc++>

  # AddressSanitizer
  $<$<BOOL:${ENABLE_ASAN}>:-fsanitize=address>

  # UndefinedBehaviorSanitizer
  $<$<BOOL:${ENABLE_UBSAN}>:-fsanitize=undefined>

  # Enable/Disable RTTI support
  $<$<COMPILE_LANGUAGE:CXX>:$<IF:$<BOOL:${ENABLE_RTTI}>,-frtti,-fno-rtti>>

  # Enable/Disable C++ exception handling
  $<$<COMPILE_LANGUAGE:CXX>:$<IF:$<BOOL:${ENABLE_EXCEPTIONS}>,-fexceptions,-fno-exceptions>>
)

#-------------------------------------------------------------------------------
# Linker Flags
#-------------------------------------------------------------------------------

add_link_options(
  -m32                # Generate code for 32-bit environment
  -Wl,--as-needed     # Only link libraries as needed
  -Wl,--gc-sections   # Perform garbage collection of unused input sections
  -Wl,--no-undefined  # Do not allow undefined symbols

  # Detailed output of the linking process
  $<$<BOOL:${ENABLE_LINK_TRACE}>:-Wl,--trace>

  # Warn about common symbols when AddressSanitizer is not enabled
  $<$<NOT:$<BOOL:${ENABLE_ASAN}>>:-Wl,--warn-common>

  # Compress debug sections using zlib
  $<$<CONFIG:RelWithDebInfo>:-Wl,--compress-debug-sections=zlib>

  # Strip all symbols
  $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:-Wl,--strip-all>

  # Discard all local symbols
  $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>,$<CONFIG:RelWithDebInfo>>:-Wl,--discard-all>

  # Link libc++ as the C++ standard library instead of libstdc++
  $<$<BOOL:${LINK_LIBCPP}>:-stdlib=libc++>

  # AddressSanitizer
  $<$<BOOL:${ENABLE_ASAN}>:-fsanitize=address>

  # UndefinedBehaviorSanitizer
  $<$<BOOL:${ENABLE_UBSAN}>:-fsanitize=undefined>

  # Link the GCC runtime library statically
  $<$<BOOL:${LINK_STATIC_GCC}>:-static-libgcc>

  # Link the C++ standard library statically
  $<$<BOOL:${LINK_STATIC_STDCPP}>:-static-libstdc++>
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

#-------------------------------------------------------------------------------
# Configure Functions
#-------------------------------------------------------------------------------

# Adds additional compile definitions for a list of targets.
#
# @param TARGETS The list of target names to add compile definitions for.
function(configure_compile_definitions)
  # Parse the arguments passed to the function
  cmake_parse_arguments("ARG" "" "" "TARGETS" ${ARGN})

  # If TARGETS is not set, print an error message and stop processing
  if(NOT ARG_TARGETS)
    message(FATAL_ERROR "TARGETS argument is required.")
  endif()

  # Loop over the list of targets, add compile definitions
  foreach(target_name IN LISTS ARG_TARGETS)
    message(STATUS "Configuring compile definitions for target \"${target_name}\"")
  endforeach()
endfunction()

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
  endforeach()
endfunction()

# Adds additional link options for a list of targets.
#
# @param TARGETS The list of target names to add link options for.
function(configure_link_options)
  # Parse the arguments passed to the function
  cmake_parse_arguments("ARG" "" "" "TARGETS" ${ARGN})

  # If TARGETS is not set, print an error message and stop processing
  if(NOT ARG_TARGETS)
    message(FATAL_ERROR "TARGETS argument is required.")
  endif()

  # Loop over the list of targets, add link options
  foreach(target_name IN LISTS ARG_TARGETS)
    message(STATUS "Configuring link options for target \"${target_name}\"")
  endforeach()
endfunction()

#-------------------------------------------------------------------------------
# Compiler-Specific Options
#-------------------------------------------------------------------------------

include_compiler_options("${CMAKE_C_COMPILER_ID}" "C")
include_compiler_options("${CMAKE_CXX_COMPILER_ID}" "CXX")
