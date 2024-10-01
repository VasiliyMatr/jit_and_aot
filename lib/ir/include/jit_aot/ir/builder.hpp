#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <jit_aot/ir/function.hpp>

namespace jit_aot::ir {

class Builder final {
    Function *m_curr_func = nullptr;
    BasicBlock *m_curr_bb = nullptr;
    BasicBlock::Instrs::iterator m_curr_bb_pos{};

  public:
    JA_NODISCARD auto *func() const noexcept { return m_curr_func; }
    void setFunc(Function *func) { m_curr_func = func; }

    auto *newBb(std::string name, bool entry = false) {
        auto new_bb =
            std::make_unique<BasicBlock>(m_curr_func, std::move(name));
        auto *out = new_bb.get();
        m_curr_func->addBb(std::move(new_bb), entry);

        m_curr_bb = out;
        m_curr_bb_pos = out->end();
        return out;
    }

    auto *newBb() { return newBb(std::to_string(m_curr_func->nextId())); }

    void setPos(BasicBlock *bb, BasicBlock::Instrs::iterator bb_pos) {
        m_curr_bb = bb;
        m_curr_bb_pos = bb_pos;
    }

    auto makeIntConst(IntType type, uint64_t value) {
        return IntConstant{type, value};
    }

    auto makeBinaryInstr(instr::BinaryInstrType type, IntValue op1,
                         IntValue op2, size_t res_id) {
        auto t1 = getIntValueType(op1);
        auto t2 = getIntValueType(op2);

        JA_ENSHURE(t1.size == t2.size);

        auto instr =
            std::make_unique<instr::IntBinaryInstr>(type, op1, op2, t1, res_id);

        auto val = instr->prod;
        m_curr_bb->insert(std::move(instr), m_curr_bb_pos);
        return val;
    }

    auto makeBinaryInstr(instr::BinaryInstrType type, IntValue op1,
                         IntValue op2) {
        return makeBinaryInstr(type, op1, op2, m_curr_func->nextId());
    }

    auto makeAdd(IntValue op1, IntValue op2) {
        return makeBinaryInstr(instr::BinaryInstrType::Add, op1, op2);
    }
    auto makeSub(IntValue op1, IntValue op2) {
        return makeBinaryInstr(instr::BinaryInstrType::Sub, op1, op2);
    }
    auto makeMul(IntValue op1, IntValue op2) {
        return makeBinaryInstr(instr::BinaryInstrType::Mul, op1, op2);
    }
    auto makeDiv(IntValue op1, IntValue op2) {
        return makeBinaryInstr(instr::BinaryInstrType::Div, op1, op2);
    }
    auto makeCmpEq(IntValue op1, IntValue op2) {
        return makeBinaryInstr(instr::BinaryInstrType::CmpEq, op1, op2);
    }
    auto makeCmpLl(IntValue op1, IntValue op2) {
        return makeBinaryInstr(instr::BinaryInstrType::CmpLl, op1, op2);
    }
    auto makeCmpGg(IntValue op1, IntValue op2) {
        return makeBinaryInstr(instr::BinaryInstrType::CmpGg, op1, op2);
    }

    auto makePhiNode(IntType type) {
        auto instr =
            std::make_unique<instr::IntPhiNode>(type, m_curr_func->nextId());

        auto out = instr.get();
        m_curr_bb->insert(std::move(instr), m_curr_bb_pos);
        return out;
    }

    void makeDirectUncondBranch(BasicBlock *dest) {
        auto instr = std::make_unique<instr::DirectUncondBranch>(dest);
        m_curr_bb->insert(std::move(instr), m_curr_bb_pos);
    }

    void makeDirectCondBranch(BasicBlock *dest, BasicBlock *fallthrough,
                              IntValue cond) {
        auto instr =
            std::make_unique<instr::DirectCondBranch>(dest, fallthrough, cond);
        m_curr_bb->insert(std::move(instr), m_curr_bb_pos);
    }

    void makeReturn(IntValue value) {
        auto instr = std::make_unique<instr::Return>(value);
        m_curr_bb->insert(std::move(instr), m_curr_bb_pos);
    }
};

} // namespace jit_aot::ir

#endif // BUILDER_HPP
