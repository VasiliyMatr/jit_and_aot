#ifndef BASIC_BLOCK_HPP
#define BASIC_BLOCK_HPP

#include <list>
#include <memory>

#include <jit_aot/ir/instr.hpp>

namespace jit_aot::ir {

class Function;

class BasicBlock final {
  public:
    using Instrs = std::list<std::unique_ptr<instr::InterfaceInstr>>;

  private:
    Function *m_function = nullptr;
    std::string m_name = "";
    Instrs m_instrs{};

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

    auto insert(std::unique_ptr<instr::InterfaceInstr> instr,
                Instrs::iterator pos) {
        return m_instrs.insert(pos, std::move(instr));
    }

    auto insert(std::unique_ptr<instr::InterfaceInstr> instr) {
        return insert(std::move(instr), m_instrs.end());
    }
};

} // namespace jit_aot::ir

#endif // BASIC_BLOCK_HPP
