cmake_minimum_required(VERSION 3.21...3.27)

#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("HLDS"
  DESCRIPTION
    "HLDS Launcher"
)

#-------------------------------------------------------------------------------
# Variable Definitions
#-------------------------------------------------------------------------------

set(PNAME_LOWER "hlds")

#-------------------------------------------------------------------------------
# Executable Definition
#-------------------------------------------------------------------------------

add_executable("${PNAME_LOWER}")
target_sources("${PNAME_LOWER}"
  PRIVATE
    "exports.cpp"
    "main.cpp"
)

set_target_properties("${PNAME_LOWER}"
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_DIR}"
    POSITION_INDEPENDENT_CODE OFF
)

if(WIN32)
  configure_file("hlds.exe.manifest.in" "${CMAKE_SOURCE_DIR}/hlds.exe.manifest" @ONLY)
  configure_file("hlds.exe.rc.in" "${CMAKE_SOURCE_DIR}/hlds.exe.rc" @ONLY)

  target_sources("${PNAME_LOWER}"
    PRIVATE
      "${CMAKE_SOURCE_DIR}/hlds.exe.manifest"
      "${CMAKE_SOURCE_DIR}/hlds.exe.rc"
  )

  target_link_options("${PNAME_LOWER}"
    PRIVATE
      "/MANIFESTUAC:level='asInvoker' uiAccess='false'"
      /MANIFEST
  )
elseif(UNIX AND NOT APPLE)
  set_target_properties("${PNAME_LOWER}" PROPERTIES
    SUFFIX "_linux"
  )

  foreach(lib IN ITEMS libgcc_s.so libstdc++.so libsvml.so libintlc.so)
    if((lib STREQUAL "libgcc_s.so" AND NOT LINK_STATIC_GCC) OR
       (lib STREQUAL "libstdc++.so" AND NOT LINK_STATIC_STDCPP) OR
       (lib MATCHES "libsvml.so|libintlc.so" AND NOT NO_INTEL_LIB AND NOT LINK_STATIC_INTEL))
      add_custom_command(
        TARGET "${PNAME_LOWER}" POST_BUILD
        WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PNAME_LOWER}>"
        COMMAND sh -c "cp \$(ldd ${PNAME_LOWER}_linux | grep '${lib}' | grep -E -o '/.+\\s') . > /dev/null 2>&1 || (exit 0)"
        VERBATIM
      )
    endif()
  endforeach()
endif()

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${PNAME_LOWER}"
  PRIVATE
    "Abstract::abstract"
    "Common::common"
    "Model::model"
    "View::console"
    "Presenter::presenter"
    "Util::util"

    $<$<PLATFORM_ID:Windows>:
      WS2_32.Lib
    >

    $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
      "c"
    >
)

#-------------------------------------------------------------------------------
# Configure Targets Properties
#-------------------------------------------------------------------------------

list(APPEND PROJECT_TARGETS "util" "common" "model" "console_view" "${PNAME_LOWER}")
configure_compile_definitions(TARGETS ${PROJECT_TARGETS})
configure_compile_options(TARGETS ${PROJECT_TARGETS})
configure_link_options(TARGETS ${PROJECT_TARGETS})

target_compile_definitions("${PNAME_LOWER}"
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
