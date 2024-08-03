#-------------------------------------------------------------------------------
# Variable definitions
#-------------------------------------------------------------------------------

set(PROJECT_NAME_CAP "Model")
set(PROJECT_NAME_LOW "model")

#-------------------------------------------------------------------------------
# Project definition
#-------------------------------------------------------------------------------

project("${PROJECT_NAME_CAP}")

#-------------------------------------------------------------------------------
# Library definition
#-------------------------------------------------------------------------------

add_library("${PROJECT_NAME_LOW}")
add_library("${PROJECT_NAME_CAP}::${PROJECT_NAME_LOW}" ALIAS "${PROJECT_NAME_LOW}")

# Common sources
target_sources("${PROJECT_NAME_LOW}"
  PRIVATE
    "cmdline_args.cpp"
    "cmdline_args.hpp"
    "console_commands.cpp"
    "console_commands.hpp"
    "server_loop.cpp"
    "server_loop.hpp"
    "server_status.hpp"
    "userinput_history.cpp"
    "userinput_history.hpp"
)

if(WIN32) # Windows sources
  target_sources("${PROJECT_NAME_LOW}"
    PRIVATE
      "windows/server_loop.cpp"
  )
else() # Unix sources
  target_sources("${PROJECT_NAME_LOW}"
    PRIVATE
      "linux/server_loop.cpp"
  )
endif()

target_compile_definitions("${PROJECT_NAME_LOW}"
  PRIVATE
    $<$<PLATFORM_ID:Windows>:
      NOMINMAX
    >
)

target_include_directories("${PROJECT_NAME_LOW}"
  PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
)

#-------------------------------------------------------------------------------
# Link libraries
#-------------------------------------------------------------------------------

target_link_libraries("${PROJECT_NAME_LOW}"
  PUBLIC
    "${LIB_COMMON}"
    "${LIB_THREADS}"
    "${LIB_UTIL}"

    $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
      "${LIB_C}" "${LIB_MATH}"
    >
)
