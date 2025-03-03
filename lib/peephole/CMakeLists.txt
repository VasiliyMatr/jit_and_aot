add_library(jit_aot_peephole STATIC)

add_library(jit_aot::peephole ALIAS jit_aot_peephole)

target_sources(jit_aot_peephole PRIVATE src/arith.cpp)

target_include_directories(jit_aot_peephole PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(jit_aot_peephole PUBLIC jit_aot::ir)
