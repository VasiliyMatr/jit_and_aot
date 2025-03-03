#include <fstream>

#include <gtest/gtest.h>

#include <jit_aot/ir/formatting.hpp>
#include <jit_aot/testing/test_funcs.hpp>

namespace jit_aot {

TEST(JA_IR_BUILDER, makeFunc1) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = testing::makeTestFunc1(builder);

    std::ofstream ir_txt{"func_001.txt"};
    ir_txt << fmt::format("{}\n", *fn);

    std::ofstream ir_dot{"func_001.dot"};
    ir_dot << fmt::format("{:dot}\n", *fn);
}

TEST(JA_IR_BUILDER, makeFunc2) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = testing::makeTestFunc2(builder);

    std::ofstream ir_txt{"func_002.txt"};
    ir_txt << fmt::format("{}\n", *fn);

    std::ofstream ir_dot{"func_002.dot"};
    ir_dot << fmt::format("{:dot}\n", *fn);
}

TEST(JA_IR_BUILDER, makeFunc3) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = testing::makeTestFunc3(builder);

    std::ofstream ir_txt{"func_003.txt"};
    ir_txt << fmt::format("{}\n", *fn);

    std::ofstream ir_dot{"func_003.dot"};
    ir_dot << fmt::format("{:dot}\n", *fn);
}

TEST(JA_IR_BUILDER, makeFunc4) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = testing::makeTestFunc4(builder);

    std::ofstream ir_txt{"func_004.txt"};
    ir_txt << fmt::format("{}\n", *fn);

    std::ofstream ir_dot{"func_004.dot"};
    ir_dot << fmt::format("{:dot}\n", *fn);
}

TEST(JA_IR_BUILDER, makeFunc5) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = testing::makeTestFunc5(builder);

    std::ofstream ir_txt{"func_005.txt"};
    ir_txt << fmt::format("{}\n", *fn);

    std::ofstream ir_dot{"func_005.dot"};
    ir_dot << fmt::format("{:dot}\n", *fn);
}

TEST(JA_IR_BUILDER, makeFunc6) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = testing::makeTestFunc6(builder);

    std::ofstream ir_txt{"func_006.txt"};
    ir_txt << fmt::format("{}\n", *fn);

    std::ofstream ir_dot{"func_006.dot"};
    ir_dot << fmt::format("{:dot}\n", *fn);
}

} // namespace jit_aot
