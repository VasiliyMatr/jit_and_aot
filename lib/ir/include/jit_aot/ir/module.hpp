#ifndef MODULE_HPP
#define MODULE_HPP

#include <jit_aot/ir/function.hpp>

namespace jit_aot::ir {

class Module final {
    std::list<IntConstant> m_consts{};
    std::list<Function> m_functions{};

  public:
    JA_NODISCARD const auto &consts() const noexcept { return m_consts; }
    JA_NODISCARD auto &consts() noexcept { return m_consts; }

    JA_NODISCARD const auto &functions() const noexcept { return m_functions; }
    JA_NODISCARD auto &functions() noexcept { return m_functions; }

    IntConstant *addIntConst(IntType type, uint64_t value) {
        return &m_consts.emplace_back(type, value);
    }

    Function *addFunc(IntType ret_type, std::vector<IntType> args_types) {
        return &m_functions.emplace_back(this, ret_type, std::move(args_types));
    }
};

} // namespace jit_aot::ir

#endif // MODULE_HPP
