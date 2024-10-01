#ifndef INSTR_HPP
#define INSTR_HPP

#include <unordered_map>

#include <jit_aot/ir/values.hpp>

namespace jit_aot::ir {

class BasicBlock;

namespace instr {

using fmt_it = fmt::format_context::iterator;

struct InterfaceInstr {
    virtual ~InterfaceInstr() = default;
    virtual fmt_it format(fmt_it out) = 0;
};

enum class BinaryInstrType {
    Add,
    Sub,
    Mul,
    Div,
    CmpEq,
    CmpLl,
    CmpGg,
};

struct IntBinaryInstr final : public InterfaceInstr {
    BinaryInstrType type;

    IntValue op1{};
    IntValue op2{};
    IntProduct prod{};

    IntBinaryInstr(BinaryInstrType type, IntValue op1, IntValue op2,
                   IntType prod_type, size_t prod_id)
        : type(type), op1(op1), op2(op2), prod{prod_type, this, prod_id} {}

    fmt_it format(fmt_it out) override;
};

struct IntPhiNode final : public InterfaceInstr {
    std::unordered_map<BasicBlock *, IntValue> map{};
    IntProduct prod{};

    IntPhiNode(IntType type, size_t prod_id) : prod{type, this, prod_id} {}

    void addToMap(std::pair<BasicBlock *, IntValue> to_add) {
        map.insert(to_add);
    }

    fmt_it format(fmt_it out) override;
};

struct DirectUncondBranch final : public InterfaceInstr {
    BasicBlock *dest = nullptr;

    DirectUncondBranch(BasicBlock *dest) : dest(dest) {}

    fmt_it format(fmt_it out) override;
};

struct DirectCondBranch final : public InterfaceInstr {
    BasicBlock *dest = nullptr;
    BasicBlock *fallthrough = nullptr;
    IntValue cond{};

    DirectCondBranch(BasicBlock *dest, BasicBlock *fallthrough, IntValue cond)
        : dest(dest), fallthrough(fallthrough), cond(cond) {}

    fmt_it format(fmt_it out) override;
};

struct Return final : public InterfaceInstr {
    IntValue value{};

    Return(IntValue value) : value(value) {}

    fmt_it format(fmt_it out) override;
};

} // namespace instr
} // namespace jit_aot::ir

#endif // INSTR_HPP
