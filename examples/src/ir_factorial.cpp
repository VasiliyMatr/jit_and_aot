#include <jit_aot/ir/builder.hpp>
#include <jit_aot/ir/formatting.hpp>

using namespace jit_aot::ir;

int main() {
    IntType i32{32};

    Builder builder{};
    Function fact_func{i32, {i32}};
    fact_func.setName("fact");

    builder.setFunc(&fact_func);

    auto v0 = fact_func.nthArg(0);

    auto const_zero = builder.makeIntConst(i32, 0);
    auto const_one = builder.makeIntConst(i32, 1);
    auto const_two = builder.makeIntConst(i32, 2);

    auto bb_init = builder.newBb("init", true);
    auto bb_cond = builder.newBb("cond");
    auto bb_loop = builder.newBb("loop");
    auto bb_ret = builder.newBb("ret");

    // Build init bb
    builder.setPos(bb_init, bb_init->end());

    auto v1 = builder.makeAdd(&const_zero, &const_two);
    auto v2 = builder.makeAdd(&const_zero, &const_one);
    builder.makeDirectUncondBranch(bb_cond);

    // Build cond bb
    builder.setPos(bb_cond, bb_cond->end());

    auto *phi1 = builder.makePhiNode(i32);
    auto v3 = phi1->result();
    phi1->addMapping(bb_init, v1);

    auto *phi2 = builder.makePhiNode(i32);
    auto v4 = phi2->result();
    phi2->addMapping(bb_init, v2);

    auto v5 = builder.makeCmpEq(v0, v3);
    builder.makeDirectCondBranch(bb_ret, bb_loop, v5);

    // Build loop bb
    builder.setPos(bb_loop, bb_loop->end());

    auto v6 = builder.makeMul(v3, v4);
    auto v7 = builder.makeAdd(v1, &const_one);

    phi1->addMapping(bb_loop, v7);
    phi2->addMapping(bb_loop, v6);

    builder.makeDirectUncondBranch(bb_cond);

    // Build ret bb
    builder.setPos(bb_ret, bb_ret->end());

    builder.makeReturn(v4);

    fmt::println("{}", fact_func);

    return 0;
}
