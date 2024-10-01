#ifndef TYPES_HPP
#define TYPES_HPP

#include <compare>

#include <jit_aot/common.hpp>

namespace jit_aot::ir {

struct IntType final {
    size_t size = 0;

    bool operator==(const IntType &) const = default;
};

} // namespace jit_aot::ir

#endif // TYPES_HPP
