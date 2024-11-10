#ifndef COMMON_FLAGS_HPP
#define COMMON_FLAGS_HPP

#include <cstdint>

namespace jit_aot::cfg::flags {

enum Flags : uint64_t {
    // General perpose marker
    kMarker = 1ull,
    // Indicates that this bb should be ignored
    kIgnored = 1ull << 1,
};

} // namespace jit_aot::flags

#endif // COMMON_FLAGS_HPP
