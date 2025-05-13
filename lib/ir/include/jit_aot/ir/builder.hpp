#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <jit_aot/ir/module.hpp>

namespace jit_aot::ir {

class Builder final {
    Module *m_curr_mod = nullptr;
    Function *m_curr_func = nullptr;
    BasicBlock *m_curr_bb = nullptr;
    BasicBlock::Instrs::iterator m_curr_bb_pos{};

  public:
    JA_NODISCARD auto *mod() const noexcept { return m_curr_mod; }
    void setMod(Module *mod) { m_curr_mod = mod; }

    JA_NODISCARD auto *func() const noexcept { return m_curr_func; }
    void setFunc(Function *func) { m_curr_func = func; }

    auto *newFunc(const InterfaceValueType *ret_type,
                  std::vector<const InterfaceValueType *> args_types) {
        m_curr_bb = nullptr;
        return m_curr_func =
                   m_curr_mod->addFunc(ret_type, std::move(args_types));
    }

    auto *newBb(std::string name, bool entry = false) {
        auto *new_bb = m_curr_func->addBb(name, entry);

        m_curr_bb = new_bb;
        m_curr_bb_pos = m_curr_bb->end();
        return new_bb;
    }

    auto *newBb() { return newBb(std::to_string(m_curr_func->nextId())); }

    void setPos(BasicBlock *bb, BasicBlock::Instrs::iterator bb_pos) {
        m_curr_bb = bb;
        m_curr_bb_pos = bb_pos;
    }

    void setPos(BasicBlock *bb) {
        m_curr_bb = bb;
        m_curr_bb_pos = m_curr_bb->end();
    }

    const auto *makeIntType(IntType type) {
        return static_cast<const IntType *>(
            m_curr_mod->addType(std::make_unique<IntType>(type)));
    }

    const auto *makePointerType(const InterfaceValueType *pointee) {
        return static_cast<const PointerType *>(
            m_curr_mod->addType(std::make_unique<PointerType>(pointee)));
    }

    const auto *makeIntConst(IntType type, uint64_t value) {
        return m_curr_mod->addIntConst(type, value);
    }

    const auto *makeIntBin(instr::InstrType type, const IntValue *op1,
                           const IntValue *op2, size_t res_id) {
        JA_ENSHURE(instr::isIntBin(type));

        const auto &t1 = op1->type();
        const auto &t2 = op2->type();

        JA_ENSHURE(t1 == t2);

        auto instr =
            std::make_unique<instr::IntBinArith>(type, op1, op2, t1, res_id);

        const auto *val = instr->result();
        m_curr_bb->insert(std::move(instr), m_curr_bb_pos);
        return val;
    }

    const auto *makeIntBin(instr::InstrType type, const IntValue *op1,
                           const IntValue *op2) {
        return makeIntBin(type, op1, op2, m_curr_func->nextId());
    }

    const auto *makeAdd(const IntValue *op1, const IntValue *op2) {
        return makeIntBin(instr::InstrType::kIntBinArithAdd, op1, op2);
    }
    const auto *makeSub(const IntValue *op1, const IntValue *op2) {
        return makeIntBin(instr::InstrType::kIntBinArithSub, op1, op2);
    }
    const auto *makeMul(const IntValue *op1, const IntValue *op2) {
        return makeIntBin(instr::InstrType::kIntBinArithMul, op1, op2);
    }
    const auto *makeDiv(const IntValue *op1, const IntValue *op2) {
        return makeIntBin(instr::InstrType::kIntBinArithDiv, op1, op2);
    }
    const auto *makeShl(const IntValue *op1, const IntValue *op2) {
        return makeIntBin(instr::InstrType::kIntBinArithShl, op1, op2);
    }
    const auto *makeOr(const IntValue *op1, const IntValue *op2) {
        return makeIntBin(instr::InstrType::kIntBinArithOr, op1, op2);
    }
    const auto *makeCmpEq(const IntValue *op1, const IntValue *op2) {
        return makeIntBin(instr::InstrType::kIntBinCmpEq, op1, op2);
    }
    const auto *makeCmpLl(const IntValue *op1, const IntValue *op2) {
        return makeIntBin(instr::InstrType::kIntBinCmpLl, op1, op2);
    }
    const auto *makeCmpGg(const IntValue *op1, const IntValue *op2) {
        return makeIntBin(instr::InstrType::kIntBinCmpGg, op1, op2);
    }

    auto *makePhiNode(IntType type) {
        auto instr =
            std::make_unique<instr::IntPhiNode>(type, m_curr_func->nextId());

        auto out = instr.get();
        m_curr_bb->insert(std::move(instr), m_curr_bb_pos);
        return out;
    }

    void makeDirectUncondBranch(BasicBlock *dest) {
        auto instr = std::make_unique<instr::BrDirectUncond>(dest);
        m_curr_bb->insertBranch(std::move(instr));
    }

    void makeDirectCondBranch(BasicBlock *dest, BasicBlock *fallthrough,
                              const IntValue *cond) {
        auto instr =
            std::make_unique<instr::BrDirectCond>(dest, fallthrough, cond);
        m_curr_bb->insertBranch(std::move(instr));
    }

    void makeReturn(const IntValue *value) {
        auto instr = std::make_unique<instr::Return>(value);
        m_curr_bb->insertBranch(std::move(instr));
    }

    void makeNullCheck(const Pointer *pointer) {
        auto instr = std::make_unique<instr::NullCheck>(pointer);
        m_curr_bb->insert(std::move(instr), m_curr_bb_pos);
    }

    void makeBoundCheck(const IntValue *index, const IntValue *bound) {
        auto instr = std::make_unique<instr::BoundCheck>(index, bound);
        m_curr_bb->insert(std::move(instr), m_curr_bb_pos);
    }
};

} // namespace jit_aot::ir

#endif // BUILDER_HPP
