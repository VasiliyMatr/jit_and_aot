#ifndef TYPES_HPP
#define TYPES_HPP

#include <compare>

#include <jit_aot/common.hpp>

namespace jit_aot::ir {

struct InterfaceValueType {
    virtual ~InterfaceValueType() = default;
    JA_NODISCARD virtual fmt_it format(fmt_it out) const = 0;
};

struct IntType final : public InterfaceValueType {
    size_t size = 0;

    IntType(size_t size) : size(size) {}

    bool operator==(const IntType &rhs) const { return size == rhs.size; }

    JA_NODISCARD fmt_it format(fmt_it out) const override {
        return fmt::format_to(out, "i{}", size);
    }
};

} // namespace jit_aot::ir

#endif // TYPES_HPP
