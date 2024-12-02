#include <fstream>

#include <gtest/gtest.h>

#include <jit_aot/cfg_analyze/loop_tree.hpp>
#include <jit_aot/testing/test_funcs.hpp>

namespace jit_aot::testing {

// WARNING: loop tree tests don't include actual checks for now. But dumps can be
// checked manually

TEST(JA_LOOP_TREE, func1) {
    ir::Builder builder{};
    auto fn = makeTestFunc1(builder);

    auto loops = cfg::getLoopTree(fn.entry());

    std::ofstream loop_tree_dot{"func_001_loop_tree.dot"};
    cfg::dumpLoopsToDot(loop_tree_dot, loops);
}

TEST(JA_LOOP_TREE, func2) {
    ir::Builder builder{};
    auto fn = makeTestFunc2(builder);

    auto loops = cfg::getLoopTree(fn.entry());

    std::ofstream loop_tree_dot{"func_002_loop_tree.dot"};
    cfg::dumpLoopsToDot(loop_tree_dot, loops);
}

TEST(JA_LOOP_TREE, func3) {
    ir::Builder builder{};
    auto fn = makeTestFunc3(builder);

    auto loops = cfg::getLoopTree(fn.entry());

    std::ofstream loop_tree_dot{"func_003_loop_tree.dot"};
    cfg::dumpLoopsToDot(loop_tree_dot, loops);
}

TEST(JA_LOOP_TREE, func4) {
    ir::Builder builder{};
    auto fn = makeTestFunc4(builder);

    auto loops = cfg::getLoopTree(fn.entry());

    std::ofstream loop_tree_dot{"func_004_loop_tree.dot"};
    cfg::dumpLoopsToDot(loop_tree_dot, loops);
}

TEST(JA_LOOP_TREE, func5) {
    ir::Builder builder{};
    auto fn = makeTestFunc5(builder);

    auto loops = cfg::getLoopTree(fn.entry());

    std::ofstream loop_tree_dot{"func_005_loop_tree.dot"};
    cfg::dumpLoopsToDot(loop_tree_dot, loops);
}

TEST(JA_LOOP_TREE, func6) {
    ir::Builder builder{};
    auto fn = makeTestFunc6(builder);

    auto loops = cfg::getLoopTree(fn.entry());

    std::ofstream loop_tree_dot{"func_006_loop_tree.dot"};
    cfg::dumpLoopsToDot(loop_tree_dot, loops);
}

} // namespace jit_aot::testing
