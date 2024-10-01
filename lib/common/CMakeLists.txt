add_library(jit_aot_common INTERFACE)
add_library(jit_aot::common ALIAS jit_aot_common)

target_include_directories(jit_aot_common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(jit_aot_common INTERFACE fmt::fmt)
