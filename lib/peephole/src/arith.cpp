#include <jit_aot/peephole/arith.hpp>

namespace jit_aot::peephole {

std::pair<bool, ir::BasicBlock::Instrs::iterator>
peephole::SubPeephole::apply(ir::BasicBlock::Instrs::iterator it,
                             ir::BasicBlock &bb) {
    auto *instr_base = it->get();

    if (instr_base->type() != ir::instr::InstrType::kIntBinArithSub) {
        return {false, it};
    }

    auto *instr = static_cast<ir::instr::IntBinArith *>(instr_base);

    auto *op1 = *instr->opBegin();
    auto *op2 = *(instr->opBegin() + 1);

    auto op1_const = dynamic_cast<const ir::IntConstant *>(op1);
    auto op2_const = dynamic_cast<const ir::IntConstant *>(op2);

    // [1] Fold constants
    if (op1_const && op2_const) {
        auto *res = bb.function()->module()->addIntConst(
            op1_const->type(), op1_const->value() - op2_const->value());

        instr->replaceUses(res);

        bb.removeInstr(it++);
        return {true, it};
    }

    // [2] x - 0 -> x
    if (op2_const && op2_const->value() == 0) {
        instr->replaceUses(op1);

        bb.removeInstr(it++);
        return {true, it};
    }

    return {false, it};
}

std::pair<bool, ir::BasicBlock::Instrs::iterator>
peephole::ShlPeephole::apply(ir::BasicBlock::Instrs::iterator it,
                             ir::BasicBlock &bb) {
    auto *instr_base = it->get();

    if (instr_base->type() != ir::instr::InstrType::kIntBinArithShl) {
        return {false, it};
    }

    auto *instr = static_cast<ir::instr::IntBinArith *>(instr_base);

    auto *op1 = *instr->opBegin();
    auto *op2 = *(instr->opBegin() + 1);

    auto op1_const = dynamic_cast<const ir::IntConstant *>(op1);
    auto op2_const = dynamic_cast<const ir::IntConstant *>(op2);

    // [1] Fold constants
    if (op1_const && op2_const) {
        auto *res = bb.function()->module()->addIntConst(
            op1_const->type(), op1_const->value() << op2_const->value());

        instr->replaceUses(res);

        bb.removeInstr(it++);
        return {true, it};
    }

    // [2] x << 0 -> x
    if (op2_const && op2_const->value() == 0) {
        instr->replaceUses(op1);

        bb.removeInstr(it++);
        return {true, it};
    }

    // [3] 0 << x -> 0
    if (op1_const && op1_const->value() == 0) {
        auto *res = bb.function()->module()->addIntConst(op1_const->type(), 0);

        instr->replaceUses(res);

        bb.removeInstr(it++);
        return {true, it};
    }

    return {false, it};
}

std::pair<bool, ir::BasicBlock::Instrs::iterator>
peephole::OrPeephole::apply(ir::BasicBlock::Instrs::iterator it,
                            ir::BasicBlock &bb) {
    auto *instr_base = it->get();

    if (instr_base->type() != ir::instr::InstrType::kIntBinArithOr) {
        return {false, it};
    }

    auto *instr = static_cast<ir::instr::IntBinArith *>(instr_base);

    auto *op1 = *instr->opBegin();
    auto *op2 = *(instr->opBegin() + 1);

    auto op1_const = dynamic_cast<const ir::IntConstant *>(op1);
    auto op2_const = dynamic_cast<const ir::IntConstant *>(op2);

    // [1] Fold constants
    if (op1_const && op2_const) {
        auto *res = bb.function()->module()->addIntConst(
            op1_const->type(), op1_const->value() | op2_const->value());

        instr->replaceUses(res);

        bb.removeInstr(it++);
        return {true, it};
    }

    // [2] x | 0 -> 0
    if (op1_const && op1_const->value() == 0) {
        auto *res = bb.function()->module()->addIntConst(op1_const->type(), 0);

        instr->replaceUses(res);

        bb.removeInstr(it++);
        return {true, it};
    }

    // [3] 0 | x -> 0
    if (op2_const && op2_const->value() == 0) {
        auto *res = bb.function()->module()->addIntConst(op2_const->type(), 0);

        instr->replaceUses(res);

        bb.removeInstr(it++);
        return {true, it};
    }

    // [4] x | x -> x
    if (op1->id() == op2->id()) {
        instr->replaceUses(op1);

        bb.removeInstr(it++);
        return {true, it};
    }

    return {false, it};
}

} // namespace jit_aot::peephole
