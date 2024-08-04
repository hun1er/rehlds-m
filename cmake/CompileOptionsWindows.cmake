#-------------------------------------------------------------------------------
# Compiler Flags
#-------------------------------------------------------------------------------

set(CMAKE_C_FLAGS
  "/DWIN32 /D_WINDOWS"
)

set(CMAKE_C_FLAGS_DEBUG
  "/Ob0 /Od /Oy- /RTC1 /D_DEBUG"
)

set(CMAKE_C_FLAGS_RELEASE
  "/O2 /Ob2 /Ot /Oy /DNDEBUG"
)

set(CMAKE_C_FLAGS_MINSIZEREL
  "/O1 /Ob1 /Os /Oy /DNDEBUG"
)

set(CMAKE_C_FLAGS_RELWITHDEBINFO
  "/O2 /Ob1 /Ot /DNDEBUG"
)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")

add_compile_options(
  /cgthreads${PROCESSOR_CORES}  # Code generation threads
  /diagnostics:caret            # Diagnostics format
  /utf-8                        # Set source and execution character sets to UTF-8
  /validate-charset             # Validate UTF-8 files for only compatible characters
  /external:W0                  # Warning level for external headers
  /fp:precise                   # Floating-point behavior
  /fp:except-                   # Floating-point behavior
  /GF                           # Eliminate duplicate strings
  /Oi                           # Generates intrinsic functions
  /Zc:inline                    # Remove unreferenced COMDAT

  # Debug information format
  $<$<CONFIG:Debug>:/Zi>

  # Function-level linking
  $<IF:$<CONFIG:Debug>,/Gy-,/Gy>

  # Whole-program global data optimization
  $<IF:$<CONFIG:Debug>,/Gw-,/Gw>

  # Control flow guard security checks
  $<$<NOT:$<CONFIG:Debug>>:/guard:cf->

  # EH continuation metadata
  $<$<NOT:$<CONFIG:Debug>>:/guard:ehcont->

  # Mitigates the performance impact of the Intel JCC erratum microcode update
  $<$<NOT:$<CONFIG:Debug>>:/QIntel-jcc-erratum>

  # Optimize for Windows applications
  $<$<AND:$<NOT:$<CONFIG:Debug>>,$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>>:/GA>

  # Enable C++ exception handling
  $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<OR:$<BOOL:${ENABLE_EXCEPTIONS}>,$<BOOL:${BUILD_UNIT_TESTS}>>>:/EHsc>

  # Enable/Disable RTTI support
  $<$<COMPILE_LANGUAGE:CXX>:$<IF:$<BOOL:${ENABLE_RTTI}>,/GR,/GR->>

  # Enables compilation of sanitizer instrumentation such as AddressSanitizer
  $<$<BOOL:${ENABLE_ASAN}>:/fsanitize=address>
)

# Enable Just My Code debugging for the Debug configuration in Visual Studio
set(CMAKE_VS_JUST_MY_CODE_DEBUGGING $<CONFIG:Debug>)

#-------------------------------------------------------------------------------
# Linker Flags
#-------------------------------------------------------------------------------

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/INCREMENTAL")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "/INCREMENTAL:NO")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "/INCREMENTAL:NO")

set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL}")
set(CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")

set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL}")
set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")

add_link_options(
  /SUBSYSTEM:CONSOLE            # Subsystem
  /NXCOMPAT                     # Compatible with the Windows Data Execution Prevention feature
  /GUARD:NO                     # Control flow guard security checks
  /WX                           # Treat linker warnings as errors

  # Generate debug info
  $<$<CONFIG:Debug>:/DEBUG:FULL>
  $<$<OR:$<CONFIG:MinSizeRel>,$<CONFIG:Release>>:/DEBUG:NONE>

  # Link-time code generation
  $<$<NOT:$<CONFIG:Debug>>:/LTCG>

  # Link optimizations
  $<$<NOT:$<CONFIG:Debug>>:/OPT:REF,ICF,LBR>

  # Ignore warning LNK4300 when AddressSanitizer is enabled
  $<$<BOOL:${ENABLE_ASAN}>:/IGNORE:4300>

  # Detailed output of the linking process
  $<$<BOOL:${ENABLE_LINK_TRACE}>:/VERBOSE:LIB>
)

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

# Static libraries linked by default
link_libraries(
  advapi32.lib
  comdlg32.lib
  gdi32.lib
  kernel32.lib
  odbc32.lib
  odbccp32.lib
  ole32.lib
  oleaut32.lib
  shell32.lib
  user32.lib
  uuid.lib
  winspool.lib
)

# Set the MSVC runtime library for use by compilers targeting the MSVC ABI
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<NOT:$<BOOL:${LINK_STATIC_MSVC_RT}>>:DLL>")

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
    target_compile_definitions("${target_name}"
      PRIVATE
        _MBCS

        # Shared library
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:
          _USRDLL
          _WINDLL
          _WINDOWS
        >

        # Static library
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,STATIC_LIBRARY>:
          _LIB
        >

        # C++ exception handling
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<NOT:$<BOOL:${ENABLE_EXCEPTIONS}>>,$<NOT:$<BOOL:${BUILD_UNIT_TESTS}>>>:
          _HAS_EXCEPTIONS=0
        >
    )
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
    target_compile_options("${target_name}"
      PRIVATE
        /W4                     # Output warning level
        /permissive-            # Standard-conformance mode
        /volatile:iso           # volatile keyword interpretation
        /options:strict         # Unrecognized compiler options are errors

        # Buffer security check.
        $<IF:$<CONFIG:Debug>,/GS,/GS->

        # Additional security checks
        #
        # Note: In recent versions of the MSVC compiler, the STL is compiled with warnings C4996.
        # The /sdl flag elevates these warnings to errors. This flag should be enabled in the future.
        #$<IF:$<CONFIG:Debug>,/sdl,/sdl->
    )
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
