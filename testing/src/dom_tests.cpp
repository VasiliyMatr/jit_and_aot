#include <fstream>

#include <gtest/gtest.h>

#include <jit_aot/cfg_analyze/dom.hpp>
#include <jit_aot/testing/test_funcs.hpp>

namespace jit_aot::testing {

// WARNING: dom tree tests don't include actual checks for now. But dumps can be
// checked manually

TEST(JA_DOM_TREE, func1) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = makeTestFunc1(builder);

    auto imm_dom_map = cfg::getImmDoms(fn->entry());

    std::ofstream dom_dot{"func_001_dom.dot"};
    dom_dot << cfg::dumpImmDomMap(imm_dom_map);
}

TEST(JA_DOM_TREE, func2) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = makeTestFunc2(builder);

    auto imm_dom_map = cfg::getImmDoms(fn->entry());

    std::ofstream dom_dot{"func_002_dom.dot"};
    dom_dot << cfg::dumpImmDomMap(imm_dom_map);
}

TEST(JA_DOM_TREE, func3) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = makeTestFunc3(builder);

    auto imm_dom_map = cfg::getImmDoms(fn->entry());

    std::ofstream dom_dot{"func_003_dom.dot"};
    dom_dot << cfg::dumpImmDomMap(imm_dom_map);
}

} // namespace jit_aot::testing
