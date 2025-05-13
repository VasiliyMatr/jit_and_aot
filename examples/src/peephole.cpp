#include <iostream>

#include <jit_aot/ir/builder.hpp>
#include <jit_aot/ir/formatting.hpp>
#include <jit_aot/peephole/arith.hpp>

using namespace jit_aot;

int main() {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    const auto *i32 = builder.makeIntType(32);

    auto *fn = builder.newFunc(i32, {i32, i32});
    fn->setName("foo");

    auto arg0 = fn->nthArgAs<ir::IntValue>(0);
    auto arg1 = fn->nthArgAs<ir::IntValue>(1);

    builder.newBb("bb1");

    auto *const_42 = builder.makeIntConst(*i32, 42);
    auto *const_10 = builder.makeIntConst(*i32, 10);
    auto *const_0 = builder.makeIntConst(*i32, 0);

    auto *v0 = builder.makeSub(arg0, const_0);
    auto *v1 = builder.makeSub(const_42, const_0);

    auto *v2 = builder.makeShl(v1, const_10);
    auto *v3 = builder.makeShl(v0, const_0);
    auto *v4 = builder.makeShl(const_0, arg1);

    auto *v5 = builder.makeOr(v0, v1);
    auto *v6 = builder.makeOr(v5, v2);
    auto *v7 = builder.makeOr(v6, v3);
    auto *v8 = builder.makeOr(v7, v4);

    builder.makeReturn(v8);

    std::cout << "IR before peepholes:\n";
    std::cout << fmt::format("{}\n", *fn);

    std::vector<std::unique_ptr<IPeephole>> peepholes{};

    peepholes.emplace_back(std::make_unique<peephole::SubPeephole>());
    peepholes.emplace_back(std::make_unique<peephole::ShlPeephole>());
    peepholes.emplace_back(std::make_unique<peephole::OrPeephole>());

    applyPeepholes(*fn, peepholes.begin(), peepholes.end(), 10);

    std::cout << "IR after peepholes:\n";
    std::cout << fmt::format("{}\n", *fn);
}
