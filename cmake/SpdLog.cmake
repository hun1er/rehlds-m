# Fast C++ logging library.
FetchContent_Declare(
  SpdLog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.12.0
)

set(SPDLOG_BUILD_ALL OFF CACHE INTERNAL
  "Build all artifacts." FORCE
)

set(SPDLOG_BUILD_SHARED OFF CACHE INTERNAL
  "Build shared library." FORCE
)

set(SPDLOG_ENABLE_PCH OFF CACHE INTERNAL
  "Build static or shared library using precompiled header to speed up compilation time." FORCE
)

set(SPDLOG_BUILD_PIC ${BUILD_SHARED_LIBS} CACHE INTERNAL
  "Build position independent code (-fPIC)." FORCE
)

set(SPDLOG_BUILD_EXAMPLE OFF CACHE INTERNAL
  "Build example." FORCE
)

set(SPDLOG_BUILD_EXAMPLE_HO OFF CACHE INTERNAL
  "Build header only example." FORCE
)

set(SPDLOG_BUILD_TESTS OFF CACHE INTERNAL
  "Build tests." FORCE
)

set(SPDLOG_BUILD_TESTS_HO OFF CACHE INTERNAL
  "Build tests using the header only version." FORCE
)

set(SPDLOG_BUILD_BENCH OFF CACHE INTERNAL
  "Build benchmarks (Requires https://github.com/google/benchmark.git to be installed)." FORCE
)

set(SPDLOG_SANITIZE_ADDRESS OFF CACHE INTERNAL
  "Enable address sanitizer in tests." FORCE
)

set(SPDLOG_BUILD_WARNINGS OFF CACHE INTERNAL
  "Enable compiler warnings." FORCE
)

set(SPDLOG_SYSTEM_INCLUDES ON CACHE INTERNAL
  "Include as system headers (skip for clang-tidy)." FORCE
)

set(SPDLOG_INSTALL OFF CACHE INTERNAL
  "Generate the install target." FORCE
)

set(SPDLOG_USE_STD_FORMAT OFF CACHE INTERNAL
  "Use std::format instead of fmt library." FORCE
)

set(SPDLOG_FMT_EXTERNAL OFF CACHE INTERNAL
  "Use external fmt library instead of bundled." FORCE
)

set(SPDLOG_FMT_EXTERNAL_HO ON CACHE INTERNAL
  "Use external fmt header-only library instead of bundled." FORCE
)

set(SPDLOG_NO_EXCEPTIONS ON CACHE INTERNAL
  "Compile with -fno-exceptions. Call abort() on any spdlog exceptions." FORCE
)

set(SPDLOG_PREVENT_CHILD_FD OFF CACHE INTERNAL
  "Prevent from child processes to inherit log file descriptors." FORCE
)

set(SPDLOG_NO_THREAD_ID ON CACHE INTERNAL
  "Prevent spdlog from querying the thread id on each log call if thread id is not needed." FORCE
)

set(SPDLOG_NO_TLS OFF CACHE INTERNAL
  "Prevent spdlog from using thread local storage." FORCE
)

set(SPDLOG_NO_ATOMIC_LEVELS ON CACHE INTERNAL
  "Prevent spdlog from using of std::atomic log levels (use only if your code never modifies log levels concurrently)." FORCE
)

set(SPDLOG_DISABLE_DEFAULT_LOGGER ON CACHE INTERNAL
  "Disable default logger creation." FORCE
)

if(WIN32)
  set(SPDLOG_WCHAR_SUPPORT OFF CACHE INTERNAL
    "Support wchar API." FORCE
  )

  set(SPDLOG_WCHAR_FILENAMES OFF CACHE INTERNAL
    "Support wchar filenames." FORCE
  )
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  set(SPDLOG_CLOCK_COARSE OFF CACHE INTERNAL
    "Use CLOCK_REALTIME_COARSE instead of the regular clock." FORCE
  )
endif()

if("${CMAKE_VERSION}" VERSION_GREATER "3.5")
  set(SPDLOG_TIDY OFF CACHE INTERNAL
    "Run Clang-Tidy." FORCE
  )
endif()
