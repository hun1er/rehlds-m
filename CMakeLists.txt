cmake_minimum_required(VERSION 3.21...3.29)

#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("ReHLDS-M"
  VERSION
    "1.3.1"

  DESCRIPTION
    "Reverse-engineered HLDS launcher"

  HOMEPAGE_URL
    "https://github.com/hun1er/rehlds-m"

  LANGUAGES
    "CXX"
)

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------

option(USE_LINKER_GOLD          "Use the Gold linker when compiling with GCC"                         ON )
option(USE_LINKER_LLD           "Use the LLD linker when compiling with Clang"                        ON )
option(USE_INTEL_SVML           "Use the Short Vector Math Library (SVML) when compiling with Intel"  ON )
option(LINK_STATIC_MSVC_RT      "Link MSVC runtime library statically"                                OFF)
option(LINK_STATIC_GCC          "Link libgcc library statically"                                      OFF)
option(LINK_STATIC_STDCPP       "Link C++ standard library statically"                                OFF)
option(LINK_LIBCPP              "Link libc++ as the C++ standard library instead of libstdc++"        OFF)
option(LINK_STATIC_INTEL        "Link Intel provided libraries statically"                            OFF)
option(NO_INTEL_LIB             "Restrict linking of all Intel specific libraries"                    OFF)
option(BUILD_UNIT_TESTS         "Build unit tests"                                                    OFF)
option(CODE_COVERAGE            "Generate a coverage target using LCOV and genhtml"                   OFF)
option(ENABLE_RTTI              "Enable support for run-time type information (RTTI)"                 OFF)
option(ENABLE_EXCEPTIONS        "Enable support for exception handling"                          ${WIN32})
option(ENABLE_CLANG_TIDY        "Enable static analysis with Clang-Tidy"                              OFF)
option(ENABLE_CPPCHECK          "Enable static analysis with Cppcheck"                                OFF)
option(ENABLE_PVS_STUDIO        "Enable static analysis with PVS-Studio"                              OFF)
option(ENABLE_IWYU              "Enable Include What You Use tool"                                    OFF)
option(ENABLE_ASAN              "Enable AddressSanitizer"                                             OFF)
option(ENABLE_UBSAN             "Enable UndefinedBehaviorSanitizer"                                   OFF)
option(ENABLE_LINK_TRACE        "Enable linker trace flag (detailed output of the linking process)"   OFF)
option(OPTIMIZE_FOR_CURRENT_CPU "Generate code optimized for the current (native) processor"          OFF)

#-------------------------------------------------------------------------------
# CMake Policies
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
# Build Configuration
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

# Building shared libraries instead of static by default
set(BUILD_SHARED_LIBS OFF)

# First search using Config mode before falling back to Module mode
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

# Set the possible build types
set(CMAKE_CONFIGURATION_TYPES "Debug" "Release" "RelWithDebInfo")

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

# Interprocedural optimization for specified build types
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)

# Dependency optimization for faster builds
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)

# Position independent code generation
set(CMAKE_POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})

# Hiding of inline functions in shared libraries
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# Export the compile commands to a JSON file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable error reporting for deprecated features
set(CMAKE_ERROR_DEPRECATED ON)

#-------------------------------------------------------------------------------
# CMake Modules and Dependencies
#-------------------------------------------------------------------------------

include("FetchContent")
include("cmake/CMakeUtilities.cmake")
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

find_package(Threads REQUIRED)
FetchContent_MakeAvailable(FmtLib)
FetchContent_MakeAvailable(SpdLog)
FetchContent_MakeAvailable(StringPy)

if(BUILD_UNIT_TESTS)
  FetchContent_MakeAvailable(GoogleTest)
  enable_testing()
endif()

#-------------------------------------------------------------------------------
# Library Definitions
#-------------------------------------------------------------------------------

set(LIB_C             "c"                     )
set(LIB_MATH          "m"                     )
set(LIB_STDC_FS       "stdc++fs"              )
set(LIB_THREADS       "Threads::Threads"      )
set(LIB_WINSOCK       "WS2_32.Lib"            )
set(LIB_FMT           "fmt::fmt"              )
set(LIB_GTEST         "GTest::gtest_main"     )
set(LIB_SPDLOG        "spdlog::spdlog"        )
set(LIB_STRINGPY      "StringPy::stringpy"    )
set(LIB_COMMON        "Common::common"        )
set(LIB_CORE          "Core::core"            )
set(LIB_MODEL         "Model::model"          )
set(LIB_PRESENTER     "Presenter::presenter"  )
set(LIB_UTIL          "Util::util"            )
set(LIB_VIEW          "View::view"            )

#-------------------------------------------------------------------------------
# Subdirectories
#-------------------------------------------------------------------------------

add_subdirectory("libs/util")
add_subdirectory("libs/common")
add_subdirectory("libs/model")
add_subdirectory("libs/presenter")
add_subdirectory("libs/view")
add_subdirectory("libs/core")
add_subdirectory("apps/hlds")

#-------------------------------------------------------------------------------
# Targets Configuration
#-------------------------------------------------------------------------------

get_targets_in_directories(
  PROJECT_TARGETS
  EXCLUDE_TESTS TRUE
  EXCLUDE_IFACE_LIBS TRUE
  DIRECTORIES "apps" "libs"
)

configure_compile_definitions(TARGETS ${PROJECT_TARGETS})
configure_compile_options(TARGETS ${PROJECT_TARGETS})
configure_link_options(TARGETS ${PROJECT_TARGETS})

#-------------------------------------------------------------------------------
# Static Analysis Tools
#-------------------------------------------------------------------------------

if(ENABLE_CLANG_TIDY)
  enable_clang_tidy(TARGETS ${PROJECT_TARGETS})
endif()

if(ENABLE_CPPCHECK)
  enable_cppcheck(TARGETS ${PROJECT_TARGETS})
endif()

if(ENABLE_PVS_STUDIO)
  enable_pvs_studio(TARGETS ${PROJECT_TARGETS})
endif()

if(ENABLE_IWYU)
  enable_iwyu(TARGETS ${PROJECT_TARGETS})
endif()
