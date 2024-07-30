#-------------------------------------------------------------------------------
# Variable definitions
#-------------------------------------------------------------------------------

set(PROJECT_NAME_LOW "hlds")
set(PROJECT_NAME_CAP "HLDS")

#-------------------------------------------------------------------------------
# Project definition
#-------------------------------------------------------------------------------

project("${PROJECT_NAME_CAP}")

#-------------------------------------------------------------------------------
# Executable definition
#-------------------------------------------------------------------------------

add_executable("${PROJECT_NAME_LOW}")
target_sources("${PROJECT_NAME_LOW}"
  PRIVATE
    "exports.cpp"
    "main.cpp"
)

set_target_properties("${PROJECT_NAME_LOW}"
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_DIR}"
    POSITION_INDEPENDENT_CODE OFF
)

if(WIN32)
  configure_file("hlds.exe.manifest.in" "${CMAKE_SOURCE_DIR}/hlds.exe.manifest" @ONLY)
  configure_file("hlds.exe.rc.in" "${CMAKE_SOURCE_DIR}/hlds.exe.rc" @ONLY)

  target_sources("${PROJECT_NAME_LOW}"
    PRIVATE
      "${CMAKE_SOURCE_DIR}/hlds.exe.manifest"
      "${CMAKE_SOURCE_DIR}/hlds.exe.rc"
  )

  target_link_options("${PROJECT_NAME_LOW}"
    PRIVATE
      "/MANIFESTUAC:level='asInvoker' uiAccess='false'"
      /MANIFEST
  )
elseif(UNIX AND NOT APPLE)
  set_target_properties("${PROJECT_NAME_LOW}" PROPERTIES
    SUFFIX "_linux"
  )

  foreach(lib IN ITEMS libgcc_s.so libstdc++.so libsvml.so libintlc.so)
    if((lib STREQUAL "libgcc_s.so" AND NOT LINK_STATIC_GCC) OR
       (lib STREQUAL "libstdc++.so" AND NOT LINK_STATIC_STDCPP) OR
       (lib MATCHES "libsvml.so|libintlc.so" AND NOT NO_INTEL_LIB AND NOT LINK_STATIC_INTEL))
      add_custom_command(
        TARGET "${PROJECT_NAME_LOW}" POST_BUILD
        WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME_LOW}>"
        COMMAND sh -c "cp \$(ldd ${PROJECT_NAME_LOW}_linux | grep '${lib}' | grep -E -o '/.+\\s') . > /dev/null 2>&1 || (exit 0)"
        VERBATIM
      )
    endif()
  endforeach()
endif()

#-------------------------------------------------------------------------------
# Link libraries
#-------------------------------------------------------------------------------

target_link_libraries("${PROJECT_NAME_LOW}"
  PRIVATE
    "${LIB_ABSTRACT}"
    "${LIB_COMMON}"
    "${LIB_MODEL}"
    "${LIB_VIEW_CONSOLE}"
    "${LIB_PRESENTER}"
    "${LIB_UTIL}"

    $<$<PLATFORM_ID:Windows>:
      "${LIB_WINSOCK}"
    >

    $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
      "${LIB_C}"
    >
)

#-------------------------------------------------------------------------------
# Configure targets properties
#-------------------------------------------------------------------------------

list(APPEND PROJECT_TARGETS "util" "common" "model" "console_view" "${PROJECT_NAME_LOW}")
configure_compile_definitions(TARGETS ${PROJECT_TARGETS})
configure_compile_options(TARGETS ${PROJECT_TARGETS})
configure_link_options(TARGETS ${PROJECT_TARGETS})

target_compile_definitions("${PROJECT_NAME_LOW}"
  PRIVATE
    $<$<PLATFORM_ID:Windows>:
      _CONSOLE
    >
)

#-------------------------------------------------------------------------------
# Static analysis
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
