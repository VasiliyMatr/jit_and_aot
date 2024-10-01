#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstdint>
#include <exception>

#include <fmt/base.h>

using std::size_t;

#define JA_NODISCARD [[nodiscard]]

#define JA_ENSHURE(cond)                                                       \
    if (!(cond)) {                                                             \
        fmt::println(stderr, "Enshure failed on {}:{}", __FILE__, __LINE__);   \
        std::terminate();                                                      \
    }

namespace jit_aot {

struct IFmtNoParseFormatter {
    constexpr fmt::format_parse_context::iterator
    parse(fmt::format_parse_context &ctx) {
        return ctx.begin();
    }
};

} // namespace jit_aot

#endif // COMMON_HPP
