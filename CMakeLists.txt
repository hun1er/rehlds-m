cmake_minimum_required(VERSION 3.21...3.27)

#-------------------------------------------------------------------------------
# Project definition
#-------------------------------------------------------------------------------

project("ReHLDS-M"
  VERSION
    "1.3.0"

  DESCRIPTION
    "Reverse-engineered HLDS"

  HOMEPAGE_URL
    "https://github.com/hun1er/rehlds-m"

  LANGUAGES
    "CXX"
)

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------

option(USE_LINKER_GOLD      "Use the Gold linker instead of the default linker when compiling with GCC."  ON )
option(USE_LINKER_LLD       "Use the LLD linker instead of the default linker when compiling with Clang." ON )
option(USE_INTEL_SVML       "Use the Short Vector Math Library (SVML) when compiling with Intel."         ON )
option(LINK_STATIC_MSVC_RT  "Link MSVC runtime library statically."                                       OFF)
option(LINK_STATIC_GCC      "Link libgcc library statically."                                             OFF)
option(LINK_STATIC_STDCPP   "Link libstdc++ library statically."                                          OFF)
option(LINK_STATIC_INTEL    "Link Intel provided libraries statically."                                   OFF)
option(NO_INTEL_LIB         "Restrict linking of all Intel specific libraries."                           OFF)
option(BUILD_UNIT_TESTS     "Build unit tests."                                                           OFF)
option(CODE_COVERAGE        "Generate a coverage target using LCOV and genhtml."                          OFF)
option(ENABLE_RTTI          "Enable support for run-time type information (RTTI)."                        OFF)
option(ENABLE_EXCEPTIONS    "Enable support for exception handling."                                 ${WIN32})
option(ENABLE_CLANG_TIDY    "Enable static analysis with Clang-Tidy to detect code issues."               OFF)
option(ENABLE_CPPCHECK      "Enable static analysis with Cppcheck to detect code issues."                 OFF)
option(ENABLE_PVS_STUDIO    "Enable static analysis with PVS-Studio to detect code issues."               OFF)
option(ENABLE_IWYU          "Enable Include What You Use tool to optimize header usage."                  OFF)
option(ENABLE_ASAN          "Enable AddressSanitizer to detect memory errors."                            OFF)
option(ENABLE_UBSAN         "Enable UndefinedBehaviorSanitizer to detect undefined behavior errors."      OFF)

#-------------------------------------------------------------------------------
# Variable definitions
#-------------------------------------------------------------------------------

include(ProcessorCount)
ProcessorCount(PROCESSOR_CORES)

if(PROCESSOR_CORES EQUAL 0)
  set(PROCESSOR_CORES 2)
endif()

if(PROCESSOR_CORES GREATER 4)
  set(PROCESSOR_CORES 4)
endif()

if(NOT BIN_OUTPUT_DIR)
  set(BIN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}-$<CONFIG>")
endif()

#-------------------------------------------------------------------------------
# CMake policy settings
#-------------------------------------------------------------------------------

if(APPLE)
  # Compiler id for Apple Clang is now "AppleClang"
  set(CMAKE_POLICY_DEFAULT_CMP0025 NEW)
endif()

# List TARGET_OBJECTS in SOURCES target property
set(CMAKE_POLICY_DEFAULT_CMP0051 NEW)

# INTERPROCEDURAL_OPTIMIZATION is enforced when enabled
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)

# option() honors normal variables
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Control generation of Position Independent Executable (PIE)
set(CMAKE_POLICY_DEFAULT_CMP0083 NEW)

# MSVC warning flags are not in CMAKE_<LANG>_FLAGS by default
set(CMAKE_POLICY_DEFAULT_CMP0092 NEW)

# Ninja generators transform DEPFILEs from add_custom_command()
set(CMAKE_POLICY_DEFAULT_CMP0116 NEW)

# MSVC RTTI flag /GR is not added to CMAKE_CXX_FLAGS by default
set(CMAKE_POLICY_DEFAULT_CMP0117 NEW)

#-------------------------------------------------------------------------------
# Compilation settings
#-------------------------------------------------------------------------------

# Building shared libraries instead of static by default
set(BUILD_SHARED_LIBS OFF)

# First search using Config mode before falling back to Module mode
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

# Set the possible build types
set(CMAKE_CONFIGURATION_TYPES "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

# Append the relative path to the RPATH list
list(APPEND CMAKE_BUILD_RPATH "$ORIGIN/.")
list(REMOVE_DUPLICATES CMAKE_BUILD_RPATH)

# Set the C language standard
set(CMAKE_C_STANDARD 17)

# Enforce the use of the C language standard
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set the visibility of symbols in C object files
set(CMAKE_C_VISIBILITY_PRESET hidden)

# Set the C++ language standard
set(CMAKE_CXX_STANDARD 17)

# Enforce the use of the C++ language standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the visibility of symbols in C++ object files
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Enable Just My Code debugging for the Debug configuration in Visual Studio
if("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CMAKE_VS_JUST_MY_CODE_DEBUGGING $<CONFIG:Debug>)
endif()

# Set the compiler options for interprocedural optimization (IPO)
if("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "IntelLLVM")
  set(CMAKE_C_COMPILE_OPTIONS_IPO "-flto=full")
endif()

# Set the compiler options for interprocedural optimization (IPO)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "IntelLLVM")
  set(CMAKE_CXX_COMPILE_OPTIONS_IPO "-flto=full")
endif()

# Interprocedural optimization for specified build types
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)

# Dependency optimization for faster builds
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)

# Position independent code generation
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Hiding of inline functions in shared libraries
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# Export the compile commands to a JSON file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable error reporting for deprecated features
set(CMAKE_ERROR_DEPRECATED ON)

# Add preprocessor definitions to the compilation of all source files
if(UNIX)
  add_compile_definitions(
    $<$<CONFIG:Debug>:_FORTIFY_SOURCE=2>
    $<$<CONFIG:Debug>:_GLIBCXX_ASSERTIONS=1>
    $<$<CONFIG:Debug>:_GLIBCXX_CONCEPT_CHECKS=1>
    $<$<CONFIG:Debug>:_GLIBCXX_DEBUG_PEDANTIC=1>
    $<$<CONFIG:Debug>:_GLIBCXX_DEBUG=1>
  )
endif()

# Set the MSVC runtime library for use by compilers targeting the MSVC ABI
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<NOT:$<BOOL:${LINK_STATIC_MSVC_RT}>>:DLL>")

#-------------------------------------------------------------------------------
# Include modules
#-------------------------------------------------------------------------------

include(FetchContent)
include("cmake/CompileOptions.cmake")
include("cmake/ClangTidy.cmake")
include("cmake/Cppcheck.cmake")
include("cmake/Iwyu.cmake")
include("cmake/PvsStudio.cmake")
include("cmake/GoogleTest.cmake")
include("cmake/FmtLib.cmake")
include("cmake/SpdLog.cmake")
include("cmake/StringPy.cmake")
include("cmake/Coverage.cmake")

#-------------------------------------------------------------------------------
# Find and fetch external dependencies
#-------------------------------------------------------------------------------

find_package(Threads REQUIRED)
FetchContent_MakeAvailable(FmtLib)
FetchContent_MakeAvailable(SpdLog)
FetchContent_MakeAvailable(StringPy)

#-------------------------------------------------------------------------------
# Add subdirectories to the build
#-------------------------------------------------------------------------------

add_subdirectory("src/abstract")
add_subdirectory("src/util")
add_subdirectory("src/common")
add_subdirectory("src/model")
add_subdirectory("src/view")
add_subdirectory("src/presenter")
add_subdirectory("src")

#-------------------------------------------------------------------------------
# Other targets
#-------------------------------------------------------------------------------

if(BUILD_UNIT_TESTS)
  enable_testing()
  add_subdirectory("tests")
endif()
