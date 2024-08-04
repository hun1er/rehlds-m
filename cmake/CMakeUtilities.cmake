# Checks if a given path is inside a specified directory.
#
# @param path The path to check.
# @param dir The directory to check against.
# @param result_var The variable to store the result (TRUE or FALSE).
function(is_path_inside_directory path dir result_var)
  # Calculate the relative path from the directory to the path.
  file(RELATIVE_PATH rel_path "${dir}" "${path}")

  # Set result_var to TRUE if the path is inside the directory, otherwise FALSE.
  if(NOT rel_path MATCHES "^\\.\\.")
    set(${result_var} TRUE PARENT_SCOPE)
  else()
    set(${result_var} FALSE PARENT_SCOPE)
  endif()
endfunction()

# Retrieves the build targets located in the specified directories,
# excluding certain targets and optionally excluding unit test targets.
#
# @param result_list The variable to store the list of targets.
# @param DIRECTORIES The directories to search for targets.
# @param EXCLUDE_TARGETS A list of targets to exclude from the result.
# @param EXCLUDE_TESTS If set, excludes targets ending with "_tests".
# @param EXCLUDE_IFACE_LIBS If set, excludes interface libraries.
function(get_targets_in_directories result_list)
  # Parse the arguments passed to the function.
  cmake_parse_arguments(ARG "" "EXCLUDE_TESTS;EXCLUDE_IFACE_LIBS" "DIRECTORIES;EXCLUDE_TARGETS" ${ARGN})

  if(NOT ARG_DIRECTORIES)
    message(FATAL_ERROR "DIRECTORIES argument is required.")
  endif()

  # Initialize the list of targets and directories to process.
  set(targets)
  set(dirs_to_process ${CMAKE_SOURCE_DIR})
  set(processed_dirs)

  # Process directories until there are no more to process.
  while(dirs_to_process)
    list(POP_FRONT dirs_to_process current_dir)

    # Skip processing if the directory was already processed.
    if(NOT current_dir IN_LIST processed_dirs)
      list(APPEND processed_dirs ${current_dir})

      # Check if the current directory is within any of the search directories.
      foreach(search_dir ${ARG_DIRECTORIES})
        is_path_inside_directory("${current_dir}" "${CMAKE_SOURCE_DIR}/${search_dir}" is_inside)

        # Retrieve and collect targets from the current directory.
        if(is_inside)
          get_property(current_targets DIRECTORY ${current_dir} PROPERTY BUILDSYSTEM_TARGETS)
          list(APPEND targets ${current_targets})
          break()
        endif()
      endforeach()

      # Add subdirectories for further processing.
      get_property(subdirs DIRECTORY ${current_dir} PROPERTY SUBDIRECTORIES)
      list(APPEND dirs_to_process ${subdirs})
    endif()
  endwhile()

  # Exclude targets based on the exclude list.
  foreach(target ${targets})
    if(target IN_LIST ARG_EXCLUDE_TARGETS)
      list(REMOVE_ITEM targets ${target})
    endif()
  endforeach()

  # Exclude unit test targets from the list.
  if(ARG_EXCLUDE_TESTS)
    list(FILTER targets EXCLUDE REGEX "_tests$")
  endif()

  # Exclude interface libraries from the list.
  if(ARG_EXCLUDE_IFACE_LIBS)
    foreach(target ${targets})
      get_target_property(target_type "${target}" TYPE)
      if("${target_type}" STREQUAL "INTERFACE_LIBRARY")
        list(REMOVE_ITEM targets "${target}")
      endif()
    endforeach()
  endif()

  # Set the result_list variable with the list of found targets.
  set(${result_list} ${targets} PARENT_SCOPE)
endfunction()
