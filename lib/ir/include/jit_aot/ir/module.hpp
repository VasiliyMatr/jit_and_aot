#ifndef MODULE_HPP
#define MODULE_HPP

#include <jit_aot/ir/function.hpp>

namespace jit_aot::ir {

class Module final {
    std::list<IntConstant> m_consts{};
    std::vector<std::unique_ptr<InterfaceValueType>> m_types{};
    std::list<Function> m_functions{};

  public:
    JA_NODISCARD const auto &consts() const noexcept { return m_consts; }
    JA_NODISCARD auto &consts() noexcept { return m_consts; }

    JA_NODISCARD const auto &types() const noexcept { return m_types; }
    JA_NODISCARD auto &types() noexcept { return m_types; }

    JA_NODISCARD const auto &functions() const noexcept { return m_functions; }
    JA_NODISCARD auto &functions() noexcept { return m_functions; }

    IntConstant *addIntConst(IntType type, uint64_t value) {
        return &m_consts.emplace_back(type, value);
    }

    InterfaceValueType *addType(std::unique_ptr<InterfaceValueType> type) {
        return m_types.emplace_back(std::move(type)).get();
    }

    Function *addFunc(const InterfaceValueType *ret_type,
                      std::vector<const InterfaceValueType *> args_types) {
        return &m_functions.emplace_back(this, ret_type, std::move(args_types));
    }
};

} // namespace jit_aot::ir

#endif // MODULE_HPP
