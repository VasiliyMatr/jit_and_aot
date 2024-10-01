#ifndef VALUES_HPP
#define VALUES_HPP

#include <variant>

#include <jit_aot/ir/types.hpp>

namespace jit_aot::ir {

struct IntConstant final {
    IntType type{};
    uint64_t value = 0;
};

namespace instr {
struct InterfaceInstr;
} // namespace instr

struct IntProduct final {
    IntType type{};
    instr::InterfaceInstr *producer = nullptr;
    size_t id = 0;
};

struct IntArgValue final {
    IntType type{};
    size_t id = 0;
};

using IntValue = std::variant<IntConstant, IntProduct, IntArgValue>;

inline auto getIntValueType(const IntValue &val) {
    return std::visit(
        [](const auto &val) -> auto{ return val.type; }, val);
}

} // namespace jit_aot::ir

#endif // VALUES_HPP
