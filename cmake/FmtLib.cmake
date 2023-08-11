# A modern formatting library.
FetchContent_Declare(
  FmtLib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        10.0.0
)

set(FMT_OS ON CACHE INTERNAL
  "Include core requiring OS (Windows/Posix)." FORCE
)

set(FMT_MODULE OFF CACHE INTERNAL
  "Build a module instead of a traditional library." FORCE
)

set(FMT_SYSTEM_HEADERS ON CACHE INTERNAL
  "Expose headers with marking them as system." FORCE
)

set(FMT_PEDANTIC OFF CACHE INTERNAL
  "Enable extra warnings and expensive tests." FORCE
)

set(FMT_WERROR OFF CACHE INTERNAL
  "Halt the compilation with an error on compiler warnings." FORCE
)

set(FMT_DOC OFF CACHE INTERNAL
  "Generate the doc target." FORCE
)

set(FMT_INSTALL OFF CACHE INTERNAL
  "Generate the install target." FORCE
)

set(FMT_TEST OFF CACHE INTERNAL
  "Generate the test target." FORCE
)

set(FMT_FUZZ OFF CACHE INTERNAL
  "Generate the fuzz target." FORCE
)

set(FMT_CUDA_TEST OFF CACHE INTERNAL
  "Generate the cuda-test target." FORCE
)
