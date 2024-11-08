#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstdint>
#include <exception>
#include <variant>

#include <fmt/format.h>

using std::size_t;

#define JA_NODISCARD [[nodiscard]]

#define JA_ENSHURE(cond)                                                       \
    if (!(cond)) {                                                             \
        fmt::println(stderr, "Enshure failed on {}:{}", __FILE__, __LINE__);   \
        std::terminate();                                                      \
    }

namespace jit_aot {

using fmt_it = fmt::format_context::iterator;

struct IFmtNoParseFormatter {
    constexpr fmt::format_parse_context::iterator
    parse(fmt::format_parse_context &ctx) {
        return ctx.begin();
    }
};

template <typename> struct VariantTag {};

template <typename T, typename V> struct GetVariantIndex;

template <typename T, typename... Ts>
struct GetVariantIndex<T, std::variant<Ts...>>
    : std::integral_constant<
          size_t, std::variant<VariantTag<Ts>...>(VariantTag<T>()).index()> {};

} // namespace jit_aot

#endif // COMMON_HPP
