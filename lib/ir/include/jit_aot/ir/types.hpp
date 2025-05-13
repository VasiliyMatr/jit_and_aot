#ifndef TYPES_HPP
#define TYPES_HPP

#include <compare>
#include <memory>

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

struct PointerType final : public InterfaceValueType {
    const InterfaceValueType *pointee_type = nullptr;

    explicit PointerType(const InterfaceValueType *pointee_type)
        : pointee_type(pointee_type) {}

    JA_NODISCARD fmt_it format(fmt_it out) const override {
        out = fmt::format_to(out, "ptr ");
        return pointee_type->format(out);
    }
};

} // namespace jit_aot::ir

#endif // TYPES_HPP
