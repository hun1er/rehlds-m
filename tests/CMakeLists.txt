cmake_minimum_required(VERSION 3.21...3.27)

#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("hlds_tests")

#-------------------------------------------------------------------------------
# Executable Definition
#-------------------------------------------------------------------------------

add_executable("${PROJECT_NAME}")
target_sources("${PROJECT_NAME}"
  PRIVATE
    "src/common/object_list.cpp"
    "src/model/cmdline_args.cpp"
    "src/model/userinput_history.cpp"
    "src/util/circular_buffer.cpp"
    "src/util/observable.cpp"
)

# Fetch GTest and link it to the test executable
FetchContent_MakeAvailable(GoogleTest)

target_link_libraries("${PROJECT_NAME}"
  PRIVATE
    "Abstract::abstract"
    "Common::common"
    "Model::model"
    "Presenter::presenter"
    "Util::util"
    "View::console"
    "GTest::gtest_main"
)

if(CODE_COVERAGE)
  generate_coverage_target("${PROJECT_NAME}")
endif()

# Register tests
gtest_discover_tests("${PROJECT_NAME}")
