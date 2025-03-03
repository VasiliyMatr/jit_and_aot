#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <jit_aot/cfg_analyze/dfs.hpp>
#include <jit_aot/testing/test_funcs.hpp>

namespace jit_aot::testing {

TEST(JA_DFS, func1) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = makeTestFunc1(builder);

    auto dfs_order = cfg::getDfsOrder(fn->entry());

    auto *A = fn->getBb("A");
    auto *B = fn->getBb("B");
    auto *C = fn->getBb("C");
    auto *D = fn->getBb("D");
    auto *E = fn->getBb("E");
    auto *F = fn->getBb("F");
    auto *G = fn->getBb("G");

    EXPECT_THAT(dfs_order, ::testing::ElementsAre(A, B, C, D, F, E, G));
}

TEST(JA_DFS, func2) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = makeTestFunc2(builder);

    auto dfs_order = cfg::getDfsOrder(fn->entry());

    auto *A = fn->getBb("A");
    auto *B = fn->getBb("B");
    auto *C = fn->getBb("C");
    auto *D = fn->getBb("D");
    auto *E = fn->getBb("E");
    auto *F = fn->getBb("F");
    auto *G = fn->getBb("G");
    auto *H = fn->getBb("H");
    auto *I = fn->getBb("I");
    auto *J = fn->getBb("J");
    auto *K = fn->getBb("K");

    EXPECT_THAT(dfs_order,
                ::testing::ElementsAre(A, B, C, D, E, F, G, H, I, K, J));
}

TEST(JA_DFS, func3) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = makeTestFunc3(builder);

    auto dfs_order = cfg::getDfsOrder(fn->entry());

    auto *A = fn->getBb("A");
    auto *B = fn->getBb("B");
    auto *C = fn->getBb("C");
    auto *D = fn->getBb("D");
    auto *E = fn->getBb("E");
    auto *F = fn->getBb("F");
    auto *G = fn->getBb("G");
    auto *H = fn->getBb("H");
    auto *I = fn->getBb("I");

    EXPECT_THAT(dfs_order, ::testing::ElementsAre(A, B, C, D, G, I, E, F, H));
}

} // namespace jit_aot::testing
