add_library(jit_aot_ir STATIC)
add_library(jit_aot::ir ALIAS jit_aot_ir)

target_sources(jit_aot_ir PRIVATE src/formatting.cpp)

target_include_directories(jit_aot_ir PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(jit_aot_ir PUBLIC fmt::fmt jit_aot::common)
