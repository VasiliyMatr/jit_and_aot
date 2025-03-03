#include <fstream>

#include <gtest/gtest.h>

#include <jit_aot/ir/builder.hpp>
#include <jit_aot/ir/formatting.hpp>
#include <jit_aot/peephole/arith.hpp>

#include <jit_aot/testing/test_funcs.hpp>

namespace jit_aot::testing {

TEST(JA_PEEPHOLE, Sub) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = builder.newFunc(i32, {i32});
    fn->setName("sub_peephole_test_func");

    auto arg0 = fn->nthArg(0);

    auto *bb1 = builder.newBb("bb1");

    auto *const_42 = builder.makeIntConst(i32, 42);
    auto *const_10 = builder.makeIntConst(i32, 10);
    auto *const_0 = builder.makeIntConst(i32, 0);

    auto *v0 = builder.makeSub(arg0, const_0);
    auto *v1 = builder.makeSub(const_42, const_10);

    builder.makeAdd(v0, v1);

    std::vector<std::unique_ptr<IPeephole>> peepholes{};
    peepholes.emplace_back(std::make_unique<peephole::SubPeephole>());

    applyPeepholes(*fn, peepholes.begin(), peepholes.end(), 10);

    auto bb_it = bb1->begin();
    auto bb_end = bb1->end();

    // Check first instr
    // Expect: add arg0, 32
    ASSERT_NE(bb_it, bb_end);

    auto &add = *bb_it++;
    EXPECT_EQ(add->type(), ir::instr::InstrType::kIntBinArithAdd);

    auto *add_op1 = *add->opBegin();
    EXPECT_EQ(add_op1->id(), arg0->id());

    auto *add_op2 = *(add->opBegin() + 1);
    auto *add_op2_const = dynamic_cast<const ir::IntConstant *>(add_op2);
    ASSERT_NE(add_op2_const, nullptr);
    EXPECT_EQ(add_op2_const->value(), 32);

    // Expect no other instrs
    EXPECT_EQ(bb_it, bb_end);
}

TEST(JA_PEEPHOLE, Shl) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = builder.newFunc(i32, {i32});
    fn->setName("shl_peephole_test_func");

    auto arg0 = fn->nthArg(0);

    auto *bb1 = builder.newBb("bb1");

    auto *const_42 = builder.makeIntConst(i32, 42);
    auto *const_10 = builder.makeIntConst(i32, 10);
    auto *const_0 = builder.makeIntConst(i32, 0);

    auto *v0 = builder.makeShl(arg0, const_0);
    auto *v1 = builder.makeShl(const_0, arg0);
    auto *v2 = builder.makeShl(const_42, const_10);

    builder.makeAdd(v0, v1);
    builder.makeReturn(v2);

    std::vector<std::unique_ptr<IPeephole>> peepholes{};
    peepholes.emplace_back(std::make_unique<peephole::ShlPeephole>());

    applyPeepholes(*fn, peepholes.begin(), peepholes.end(), 10);

    auto bb_it = bb1->begin();
    auto bb_end = bb1->end();

    // Check first instr
    // Expect: add arg0, 0
    ASSERT_NE(bb_it, bb_end);

    auto &add = *bb_it++;
    EXPECT_EQ(add->type(), ir::instr::InstrType::kIntBinArithAdd);

    auto *add_op1 = *add->opBegin();
    EXPECT_EQ(add_op1->id(), arg0->id());

    auto *add_op2 = *(add->opBegin() + 1);
    auto *add_op2_const = dynamic_cast<const ir::IntConstant *>(add_op2);
    ASSERT_NE(add_op2_const, nullptr);
    EXPECT_EQ(add_op2_const->value(), 0);

    // Check second instr
    // Expect: ret 42<<10
    auto &ret = *bb_it++;
    EXPECT_EQ(ret->type(), ir::instr::InstrType::kReturn);

    auto *ret_op = *ret->opBegin();
    auto *ret_op_const = dynamic_cast<const ir::IntConstant *>(ret_op);
    ASSERT_NE(ret_op_const, nullptr);
    EXPECT_EQ(ret_op_const->value(), 42 << 10);

    // Expect no other instrs
    EXPECT_EQ(bb_it, bb_end);
}

TEST(JA_PEEPHOLE, Or) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    auto *fn = builder.newFunc(i32, {i32});
    fn->setName("or_peephole_test_func");

    auto arg0 = fn->nthArg(0);

    auto *bb1 = builder.newBb("bb1");

    auto *const_42 = builder.makeIntConst(i32, 42);
    auto *const_10 = builder.makeIntConst(i32, 10);
    auto *const_0 = builder.makeIntConst(i32, 0);

    auto *v0 = builder.makeOr(arg0, const_0);
    auto *v1 = builder.makeOr(const_0, arg0);
    auto *v2 = builder.makeOr(arg0, arg0);
    auto *v3 = builder.makeOr(const_42, const_10);

    builder.makeAdd(v0, v1);
    builder.makeAdd(v2, v3);

    std::vector<std::unique_ptr<IPeephole>> peepholes{};
    peepholes.emplace_back(std::make_unique<peephole::OrPeephole>());

    applyPeepholes(*fn, peepholes.begin(), peepholes.end(), 10);

    auto bb_it = bb1->begin();
    auto bb_end = bb1->end();

    // Check first instr
    // Expect: add 0, 0
    ASSERT_NE(bb_it, bb_end);

    auto &add1 = *bb_it++;
    EXPECT_EQ(add1->type(), ir::instr::InstrType::kIntBinArithAdd);

    auto *add1_op1 = *add1->opBegin();
    auto *add1_op1_const = dynamic_cast<const ir::IntConstant *>(add1_op1);
    ASSERT_NE(add1_op1_const, nullptr);
    EXPECT_EQ(add1_op1_const->value(), 0);

    auto *add1_op2 = *(add1->opBegin() + 1);
    auto *add1_op2_const = dynamic_cast<const ir::IntConstant *>(add1_op2);
    ASSERT_NE(add1_op2_const, nullptr);
    EXPECT_EQ(add1_op2_const->value(), 0);

    // Check second instr
    // Expect: add arg0, 42|10
    auto &add2 = *bb_it++;
    EXPECT_EQ(add2->type(), ir::instr::InstrType::kIntBinArithAdd);

    auto *add2_op1 = *add2->opBegin();
    EXPECT_EQ(add2_op1->id(), arg0->id());

    auto *add2_op2 = *(add2->opBegin() + 1);
    auto *add2_op2_const = dynamic_cast<const ir::IntConstant *>(add2_op2);
    ASSERT_NE(add2_op2_const, nullptr);
    EXPECT_EQ(add2_op2_const->value(), 42 | 10);

    // Expect no other instrs
    EXPECT_EQ(bb_it, bb_end);
}

} // namespace jit_aot::testing
