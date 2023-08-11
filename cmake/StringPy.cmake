# A C++17 library providing a set of string manipulation functions.
FetchContent_Declare(
  StringPy
  GIT_REPOSITORY https://github.com/hun1er/stringpy.git
  GIT_TAG        v1.0.2
)

set(STRINGPY_INSTALL OFF CACHE INTERNAL
  "Generate install target." FORCE
)

set(STRINGPY_BUILD_DOCS OFF CACHE INTERNAL
  "Generate documentation." FORCE
)

set(STRINGPY_BUILD_EXAMPLES OFF CACHE INTERNAL
  "Build usage examples." FORCE
)

set(STRINGPY_BUILD_TESTS OFF CACHE INTERNAL
  "Build unit tests." FORCE
)

set(STRINGPY_CODE_COVERAGE OFF CACHE INTERNAL
  "Generate a coverage target using LCOV and genhtml." FORCE
)

set(STRINGPY_SYSTEM_HEADERS ON CACHE INTERNAL
  "Treat headers as system headers to suppress warnings." FORCE
)
