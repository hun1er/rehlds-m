#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("HLDS Launcher")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "hlds")
add_executable("${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

set(HPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/include/${TARGET_NAME}")
set(CPP_SOURCES_DIR "${PROJECT_SOURCE_DIR}/src/${TARGET_NAME}")

target_sources("${TARGET_NAME}"
  PRIVATE
    "${CPP_SOURCES_DIR}/${TARGET_NAME}.main.cpp"
)

if(WIN32)
  set(EXE_RESOURCES "${TARGET_NAME}.exe.rc")
  set(EXE_MANIFEST "${TARGET_NAME}.exe.manifest")

  configure_file("${EXE_RESOURCES}.in" "${PROJECT_BINARY_DIR}/${EXE_RESOURCES}" @ONLY)
  configure_file("${EXE_MANIFEST}.in" "${PROJECT_BINARY_DIR}/${EXE_MANIFEST}" @ONLY)

  target_sources("${TARGET_NAME}"
    PRIVATE
      "${PROJECT_BINARY_DIR}/${EXE_RESOURCES}"
      "${PROJECT_BINARY_DIR}/${EXE_MANIFEST}"
  )
endif()

#-------------------------------------------------------------------------------
# Target Properties
#-------------------------------------------------------------------------------

set_target_properties("${TARGET_NAME}"
  PROPERTIES
    ENABLE_EXPORTS ON
    RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_DIR}"
    POSITION_INDEPENDENT_CODE OFF
)

if(UNIX AND NOT APPLE)
  set(SUFFIX_NAME "_linux")

  set_target_properties("${TARGET_NAME}"
    PROPERTIES
      SUFFIX "${SUFFIX_NAME}"
  )
endif()

#-------------------------------------------------------------------------------
# Linker Flags
#-------------------------------------------------------------------------------

target_link_options("${TARGET_NAME}"
  PRIVATE
    $<$<PLATFORM_ID:Windows>:
      "/MANIFESTUAC:level='asInvoker' uiAccess='false'"
      /MANIFEST
    >
)

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${TARGET_NAME}"
  PRIVATE
    "${LIB_COMMON}"
    "${LIB_CORE}"
    "${LIB_MODEL}"
    "${LIB_PRESENTER}"
    "${LIB_VIEW}"
)

#-------------------------------------------------------------------------------
# Post-Build Actions
#-------------------------------------------------------------------------------

if(UNIX AND NOT APPLE)
  foreach(lib IN ITEMS libgcc_s.so libstdc++.so libsvml.so libintlc.so)
    if((lib STREQUAL "libgcc_s.so" AND NOT LINK_STATIC_GCC) OR
        (lib STREQUAL "libstdc++.so" AND NOT LINK_STATIC_STDCPP) OR
        (lib MATCHES "libsvml.so|libintlc.so" AND NOT NO_INTEL_LIB AND NOT LINK_STATIC_INTEL))
      add_custom_command(
        TARGET "${TARGET_NAME}" POST_BUILD
        WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>"
        COMMAND sh -c "cp \$(ldd '${TARGET_NAME}${SUFFIX_NAME}' | grep '${lib}' | grep -E -o '/.+\\s') . > /dev/null 2>&1 || (exit 0)"
        VERBATIM
      )
    endif()
  endforeach()
endif()
