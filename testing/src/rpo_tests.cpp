#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <jit_aot/cfg_analyze/rpo.hpp>
#include <jit_aot/testing/test_funcs.hpp>

namespace jit_aot::testing {

// Test that for each bb preds are traversed first
void testRpo(const std::vector<ir::BasicBlock *> rpo) {
    for (auto it = rpo.cbegin(), end = rpo.cend(); it != end; ++it) {
        const auto *bb = *it;

        for (auto it_next = it + 1; it_next != end; ++it_next) {
            const auto *next = *it_next;

            std::for_each(bb->predBegin(), bb->predEnd(),
                          [next](const auto *pred) { EXPECT_NE(next, pred); });
        }
    }
}

TEST(JA_RPO, func1) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = makeTestFunc1(builder);

    auto rpo = cfg::getRpoOrder(fn->entry(), fn->getBbNum());
    ASSERT_EQ(rpo.size(), fn->getBbNum());
    testRpo(rpo);
}

TEST(JA_RPO, func2) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = makeTestFunc2(builder);

    auto rpo_order = cfg::getRpoOrder(fn->entry(), fn->getBbNum());

    auto rpo = cfg::getRpoOrder(fn->entry(), fn->getBbNum());
    ASSERT_EQ(rpo.size(), fn->getBbNum());
    testRpo(rpo);
}

TEST(JA_RPO, func3) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = makeTestFunc3(builder);

    auto rpo_order = cfg::getRpoOrder(fn->entry(), fn->getBbNum());

    auto rpo = cfg::getRpoOrder(fn->entry(), fn->getBbNum());
    ASSERT_EQ(rpo.size(), fn->getBbNum());
    testRpo(rpo);
}

} // namespace jit_aot::testing
