add_library(jit_aot_cfg_analyze INTERFACE)
add_library(jit_aot::cfg_analyze ALIAS jit_aot_cfg_analyze)

target_include_directories(jit_aot_cfg_analyze INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(jit_aot_cfg_analyze INTERFACE fmt::fmt jit_aot::common jit_aot::ir)
