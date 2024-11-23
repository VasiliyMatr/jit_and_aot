add_library(jit_aot_testing INTERFACE)
add_library(jit_aot::testing ALIAS jit_aot_testing)

target_include_directories(jit_aot_testing INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(jit_aot_testing
  INTERFACE
    GTest::gtest_main
    GTest::gmock
    pthread
    fmt::fmt
    jit_aot::common
    jit_aot::cfg_analyze
)

add_executable(builder_tests src/builder_tests.cpp)
target_link_libraries(builder_tests
  PRIVATE
    jit_aot::testing
    jit_aot::ir
)

add_executable(dfs_tests src/dfs_tests.cpp)
target_link_libraries(dfs_tests
  PRIVATE
    jit_aot::testing
    jit_aot::cfg_analyze
)

add_executable(rpo_tests src/rpo_tests.cpp)
target_link_libraries(rpo_tests
  PRIVATE
    jit_aot::testing
    jit_aot::cfg_analyze
)

add_executable(dom_tests src/dom_tests.cpp)
target_link_libraries(dom_tests
  PRIVATE
    jit_aot::testing
    jit_aot::cfg_analyze
)

add_executable(loop_tree_tests src/loop_tree_tests.cpp)
target_link_libraries(loop_tree_tests
  PRIVATE
    jit_aot::testing
    jit_aot::cfg_analyze
)

include(GoogleTest)
gtest_discover_tests(builder_tests)
gtest_discover_tests(dfs_tests)
gtest_discover_tests(rpo_tests)
gtest_discover_tests(dom_tests)
gtest_discover_tests(loop_tree_tests)
