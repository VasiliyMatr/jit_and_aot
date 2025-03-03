#ifndef INSTR_HPP
#define INSTR_HPP

#include <algorithm>
#include <array>
#include <optional>
#include <unordered_map>
#include <vector>

#include <jit_aot/ir/values.hpp>

namespace jit_aot::ir {

class BasicBlock;

namespace instr {

enum class InstrType {
    kIntBinArithAdd,
    kIntBinArithSub,
    kIntBinArithMul,
    kIntBinArithDiv,
    kIntBinArithShl,
    kIntBinArithOr,

    kIntBinCmpLl,
    kIntBinCmpGg,
    kIntBinCmpEq,

    kIntPhiNode,

    kBrDirectUncond,
    kBrDirectCond,

    kReturn,

    kInvalid,
};

inline constexpr bool isIntBin(InstrType instr_type) {
    switch (instr_type) {
    case InstrType::kIntBinArithAdd:
    case InstrType::kIntBinArithSub:
    case InstrType::kIntBinArithMul:
    case InstrType::kIntBinArithDiv:
    case InstrType::kIntBinArithShl:
    case InstrType::kIntBinArithOr:
    case InstrType::kIntBinCmpLl:
    case InstrType::kIntBinCmpGg:
    case InstrType::kIntBinCmpEq:
        return true;

    default:
        return false;
    }
}

inline constexpr bool isPhiNode(InstrType instr_type) {
    return instr_type == InstrType::kIntPhiNode;
}

inline constexpr bool isBranch(InstrType instr_type) {
    switch (instr_type) {
    case InstrType::kBrDirectUncond:
    case InstrType::kBrDirectCond:
    case InstrType::kReturn:
        return true;
    default:
        return false;
    }
}

// Base class for all instructions
class Instr {
  protected:
    InstrType m_type = InstrType::kInvalid;
    std::vector<const Value *> m_ops{};
    std::vector<Instr *> m_users{};

  public:
    Instr(InstrType type, std::vector<const Value *> ops)
        : m_type{type}, m_ops(std::move(ops)) {
        addThisAsUser();
    }

    virtual ~Instr() = 0;

    // Get instr type
    JA_NODISCARD auto type() const noexcept { return m_type; }

    JA_NODISCARD auto opBegin() noexcept { return m_ops.begin(); }
    JA_NODISCARD auto opEnd() noexcept { return m_ops.end(); }
    JA_NODISCARD auto opBegin() const noexcept { return m_ops.begin(); }
    JA_NODISCARD auto opEnd() const noexcept { return m_ops.end(); }

    // Get instr result ptr (== nullptr if instr don't have result)
    JA_NODISCARD virtual const Value *result() const noexcept {
        return nullptr;
    }

    // Get instr users begin
    JA_NODISCARD auto usersBegin() noexcept { return m_users.begin(); }
    // Get instr users end
    JA_NODISCARD auto usersEnd() noexcept { return m_users.end(); }

    // Add instr user
    void addUser(Instr *user) {
        JA_ENSHURE(result() &&
                   "Can not add user for instruction without result");
        m_users.push_back(user);
    }

    // Remove given user. Does nothing if such user isn't present.
    // Return true if user was removed and false otherwise
    bool removeUser(Instr *user) { return std::erase(m_users, user); }

    void clearUsers() { m_users.clear(); }

    // Format instr to `out` and return new iterator
    JA_NODISCARD virtual fmt_it format(fmt_it out) const;

  private:
    // Add this as user for all operands producers
    void addThisAsUser() {
        for (const auto &op : m_ops) {
            if (auto *prod = op->producer()) {
                prod->addUser(this);
            }
        }
    }
};

inline Instr::~Instr() = default;

// Base class for instructions with int result
class IntResInstr : public Instr {
    IntValue m_result;

  public:
    IntResInstr(InstrType type, std::vector<const Value *> ops,
                IntType res_type, ValueId res_id)
        : Instr(type, std::move(ops)), m_result{res_type, res_id, this} {}

    JA_NODISCARD const IntValue *result() const noexcept override {
        return &m_result;
    }

    void replaceUses(const Value *new_value) {
        if (auto *prod = new_value->producer()) {
            replaceUses(new_value, static_cast<IntResInstr *>(prod));
        } else {
            replaceUses(new_value, nullptr);
        }
    }

  private:
    void replaceUses(const Value *new_value, IntResInstr *new_producer) {
        auto res_id = m_result.id();

        for (auto *user : m_users) {
            for (auto it = user->opBegin(), end = user->opEnd(); it != end;
                 ++it) {
                const auto *&op = *it;
                if (op->id() == res_id) {
                    op = new_value;
                }
            }

            if (new_producer != nullptr) {
                new_producer->addUser(user);
            }
        }

        clearUsers();
    }
};

// Instruction that operates on two ints and produces int
class IntBinArith final : public IntResInstr {
  public:
    IntBinArith(InstrType type, const Value *op1, const Value *op2,
                IntType res_type, ValueId res_id)
        : IntResInstr(type, {op1, op2}, res_type, res_id) {}
};

class IntPhiNode final : public IntResInstr {
    std::unordered_map<BasicBlock *, const Value *> m_map{};

  public:
    IntPhiNode(IntType res_type, ValueId res_id)
        : IntResInstr(InstrType::kIntPhiNode, {}, res_type, res_id) {}

    void addMapping(BasicBlock *bb, const Value *value) {
        m_ops.push_back(value);
        JA_ENSHURE(m_map.try_emplace(bb, value).second &&
                   "Duplicated BasicBlock ptr in IntPhiNode");
    }

    JA_NODISCARD const auto &getMap() const noexcept { return m_map; }

    JA_NODISCARD fmt_it format(fmt_it out) const override;
};

struct BrDirectUncond final : public Instr {
    BasicBlock *dest = nullptr;

    BrDirectUncond(BasicBlock *dest)
        : Instr(InstrType::kBrDirectUncond, {}), dest(dest) {}

    JA_NODISCARD fmt_it format(fmt_it out) const override;
};

struct BrDirectCond final : public Instr {
    BasicBlock *dest = nullptr;
    BasicBlock *fallthrough = nullptr;

    BrDirectCond(BasicBlock *dest, BasicBlock *fallthrough,
                 const IntValue *cond)
        : Instr(InstrType::kBrDirectCond, {cond}), dest(dest),
          fallthrough(fallthrough) {}

    JA_NODISCARD fmt_it format(fmt_it out) const override;
};

struct Return final : public Instr {
    Return(const IntValue *ret_val) : Instr(InstrType::kReturn, {ret_val}) {}
};

} // namespace instr
} // namespace jit_aot::ir

#endif // INSTR_HPP
