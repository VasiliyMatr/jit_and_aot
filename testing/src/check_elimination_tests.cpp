#include <fstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <jit_aot/check_elimination.hpp>
#include <jit_aot/ir/formatting.hpp>
#include <jit_aot/testing/test_funcs.hpp>

namespace jit_aot {

TEST(JA_CHECK_ELIMINATION, Basic) {
    ir::Module mod;
    ir::Builder builder{};
    builder.setMod(&mod);

    const auto *i32 = builder.makeIntType(32);
    const auto *i32_ptr = builder.makePointerType(i32);

    auto *fn = builder.newFunc(i32, {i32_ptr, i32, i32});
    fn->setName("test_func");

    auto *v0 = fn->nthArgAs<ir::Pointer>(0);
    auto *v1 = fn->nthArgAs<ir::IntValue>(1);
    auto *v2 = fn->nthArgAs<ir::IntValue>(2);

    auto *A = builder.newBb("A", true);
    auto *B = builder.newBb("B");

    builder.setPos(A);
    builder.makeNullCheck(v0);
    builder.makeBoundCheck(v1, v2);
    builder.makeNullCheck(v0);
    builder.makeBoundCheck(v1, v2);
    builder.makeDirectUncondBranch(B);

    builder.setPos(B);
    builder.makeNullCheck(v0);
    builder.makeBoundCheck(v1, v2);
    builder.makeReturn(v1);

    check_elimination::eliminateChecks(*fn);

    auto bb_it = A->begin();
    auto bb_end = A->end();

    // Check first instr
    // Expect: null_check v0
    ASSERT_NE(bb_it, bb_end);
    auto &null_check = *bb_it++;
    ASSERT_EQ(null_check->type(), ir::instr::InstrType::kNullCheck);
    EXPECT_EQ(null_check->getNthOpId(0), v0->id());

    // Check second instr
    // Expect: bound_check v1, v2
    ASSERT_NE(bb_it, bb_end);
    auto &bound_check = *bb_it++;
    ASSERT_EQ(bound_check->type(), ir::instr::InstrType::kBoundCheck);
    EXPECT_EQ(bound_check->getNthOpId(0), v1->id());
    EXPECT_EQ(bound_check->getNthOpId(1), v2->id());

    // Check third instr
    // Expect: b B
    ASSERT_NE(bb_it, bb_end);
    auto &branch = *bb_it++;
    ASSERT_EQ(branch->type(), ir::instr::InstrType::kBrDirectUncond);
    auto *branch_ptr = dynamic_cast<ir::instr::BrDirectUncond *>(branch.get());
    EXPECT_NE(branch_ptr, nullptr);
    EXPECT_EQ(branch_ptr->dest, B);

    // Expect no other instrs in A
    EXPECT_EQ(bb_it, bb_end);

    bb_it = B->begin();
    bb_end = B->end();

    // Check first instr
    // Expect: ret v1
    ASSERT_NE(bb_it, bb_end);
    auto &ret = *bb_it++;
    ASSERT_EQ(ret->type(), ir::instr::InstrType::kReturn);
    EXPECT_EQ(ret->getNthOpId(0), v1->id());

    // Expect no other instrs in B
    EXPECT_EQ(bb_it, bb_end);
}

} // namespace jit_aot
