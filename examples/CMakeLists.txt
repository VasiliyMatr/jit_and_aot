# Basic example with manual factorial function IR build
add_executable(ir_factorial src/ir_factorial.cpp)
target_link_libraries(ir_factorial PRIVATE jit_aot::ir)
