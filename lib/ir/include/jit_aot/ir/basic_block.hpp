#ifndef BASIC_BLOCK_HPP
#define BASIC_BLOCK_HPP

#include <list>
#include <memory>
#include <vector>

#include <jit_aot/ir/instr.hpp>

namespace jit_aot::ir {

class Function;
class Loop;

class BasicBlock final {
  public:
    // Use list for fast insertion and remove
    using Instrs = std::list<std::unique_ptr<instr::Instr>>;

  private:
    Function *m_function = nullptr;
    std::string m_name = "";
    Instrs m_instrs{};
    bool m_have_branch = false;

    std::vector<BasicBlock *> m_preds{};
    std::vector<BasicBlock *> m_succs{};

    // Flags placeholder for traversals
    uint64_t m_flags = 0;

  public:
    BasicBlock(Function *function, std::string name)
        : m_function(function), m_name(std::move(name)) {}

    void setName(std::string name) noexcept { m_name = std::move(name); }
    JA_NODISCARD std::string_view name() const noexcept { return m_name; }

    JA_NODISCARD Function *function() const noexcept { return m_function; }

    JA_NODISCARD auto begin() noexcept { return m_instrs.begin(); }
    JA_NODISCARD auto end() noexcept { return m_instrs.end(); }
    JA_NODISCARD auto begin() const noexcept { return m_instrs.cbegin(); }
    JA_NODISCARD auto end() const noexcept { return m_instrs.cend(); }

    JA_NODISCARD auto predBegin() noexcept { return m_preds.begin(); }
    JA_NODISCARD auto predEnd() noexcept { return m_preds.end(); }
    JA_NODISCARD auto predBegin() const noexcept { return m_preds.cbegin(); }
    JA_NODISCARD auto predEnd() const noexcept { return m_preds.cend(); }

    JA_NODISCARD auto succBegin() noexcept { return m_succs.begin(); }
    JA_NODISCARD auto succEnd() noexcept { return m_succs.end(); }
    JA_NODISCARD auto succBegin() const noexcept { return m_succs.cbegin(); }
    JA_NODISCARD auto succEnd() const noexcept { return m_succs.cend(); }

    void addPredecessor(BasicBlock *pred) { m_preds.push_back(pred); }

    // Remove given predecessor. Does nothing if such predecessor isn't present.
    // Return true if predecessor was removed and false otherwise
    bool removePredecessor(BasicBlock *pred) {
        return std::erase(m_preds, pred);
    }

    // Should not be called for branch insertion
    auto insert(std::unique_ptr<instr::Instr> &&instr, Instrs::iterator pos) {
        JA_ENSHURE(!instr::isBranch(instr->type()));
        return m_instrs.insert(pos, std::move(instr));
    }

    // Should not be called for branch insertion
    auto insert(std::unique_ptr<instr::Instr> &&instr) {
        JA_ENSHURE(!instr::isBranch(instr->type()));
        return insert(std::move(instr), m_instrs.end());
    }

  private:
    // Insert branch or return with successors setup
    auto insertBranchRaw(std::unique_ptr<instr::Instr> &&b,
                         std::vector<BasicBlock *> &&b_succ) {
        JA_ENSHURE(!m_have_branch);
        JA_ENSHURE(instr::isBranch(b->type()));

        auto out = m_instrs.emplace(m_instrs.end(), std::move(b));
        m_succs = std::move(b_succ);
        m_have_branch = true;

        for (auto succ : m_succs) {
            succ->addPredecessor(this);
        }

        return out;
    }

  public:
    auto insertBranch(std::unique_ptr<instr::BrDirectUncond> &&b) {
        std::vector<BasicBlock *> succ{b->dest};
        return insertBranchRaw(std::move(b), std::move(succ));
    }

    auto insertBranch(std::unique_ptr<instr::BrDirectCond> &&b) {
        std::vector<BasicBlock *> succ{b->dest, b->fallthrough};
        return insertBranchRaw(std::move(b), std::move(succ));
    }

    auto insertBranch(std::unique_ptr<instr::Return> &&b) {
        return insertBranchRaw(std::move(b), {});
    }

    void removeInstr(Instrs::iterator pos) { m_instrs.erase(pos); }

    JA_NODISCARD auto flags() const noexcept { return m_flags; }
    JA_NODISCARD bool testFlags(uint64_t mask) const noexcept {
        return m_flags & mask;
    }

    void resetFlags() noexcept { m_flags = 0; }
    auto setFlags(uint64_t flags) noexcept { return m_flags = flags; }
    auto addFlags(uint64_t flags) noexcept { return m_flags |= flags; }
};

} // namespace jit_aot::ir

#endif // BASIC_BLOCK_HPP
