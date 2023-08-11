# Includes compiler options for a specific compiler and language.
#
# @param compiler_id The ID of the compiler to include options for.
# @param lang The language to include options for.
function(include_compiler_options compiler_id lang)
  # Return early if the compiler ID is not provided or is empty.
  if(NOT compiler_id OR "${compiler_id}" STREQUAL "")
    return()
  endif()

  # Construct the file name and path for the compiler options file.
  set(compiler_options_file_name "CompileOptions${CMAKE_SYSTEM_NAME}${compiler_id}.cmake")
  set(compiler_options_file_path "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${compiler_options_file_name}")

  # Attempt to include the compiler options file.
  include("${compiler_options_file_path}" OPTIONAL RESULT_VARIABLE include_compiler_options_result)

  # Check if the include was successful and print a status message.
  if(include_compiler_options_result)
    message(STATUS
      "Included \"${compiler_options_file_path}\" for ${lang} language."
    )
  else()
    message(STATUS
      "No \"${compiler_options_file_path}\" file found for language ${lang}. Default options will be used."
    )
  endif()
endfunction()

# Set the compile options file name and path
set(COMPILE_OPTIONS_FILE_NAME "CompileOptions${CMAKE_SYSTEM_NAME}.cmake")
set(COMPILE_OPTIONS_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${COMPILE_OPTIONS_FILE_NAME}")

# Include the CompileOptions${CMAKE_SYSTEM_NAME}.cmake file
include("${COMPILE_OPTIONS_FILE_PATH}" OPTIONAL RESULT_VARIABLE INCLUDE_COMPILER_OPTIONS_RESULT)

# If the file was successfully included, print a message indicating this
# Otherwise, print a message indicating that default options will be used
if(INCLUDE_COMPILER_OPTIONS_RESULT)
  message(STATUS
    "Included \"${COMPILE_OPTIONS_FILE_PATH}\""
  )
else()
  message(STATUS
    "No \"${COMPILE_OPTIONS_FILE_PATH}\" file found. Default options will be used."
  )
endif()
