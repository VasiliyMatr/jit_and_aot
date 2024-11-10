#include <fstream>

#include <gtest/gtest.h>

#include <jit_aot/ir/formatting.hpp>
#include <jit_aot/testing/test_funcs.hpp>

namespace jit_aot {

TEST(JA_IR_BUILDER, makeFunc1) {
    ir::Builder builder{};
    auto fn = testing::makeTestFunc1(builder);

    std::ofstream ir_txt{"func_001.txt"};
    ir_txt << fmt::format("{}\n", fn);

    std::ofstream ir_dot{"func_001.dot"};
    ir_dot << fmt::format("{:dot}\n", fn);
}

TEST(JA_IR_BUILDER, makeFunc2) {
    ir::Builder builder{};
    auto fn = testing::makeTestFunc2(builder);

    std::ofstream ir_txt{"func_002.txt"};
    ir_txt << fmt::format("{}\n", fn);

    std::ofstream ir_dot{"func_002.dot"};
    ir_dot << fmt::format("{:dot}\n", fn);
}

TEST(JA_IR_BUILDER, makeFunc3) {
    ir::Builder builder{};
    auto fn = testing::makeTestFunc3(builder);

    std::ofstream ir_txt{"func_003.txt"};
    ir_txt << fmt::format("{}\n", fn);

    std::ofstream ir_dot{"func_003.dot"};
    ir_dot << fmt::format("{:dot}\n", fn);
}

} // namespace jit_aot
