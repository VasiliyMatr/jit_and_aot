#ifndef COMMON_FLAGS_HPP
#define COMMON_FLAGS_HPP

#include <cstdint>

namespace jit_aot::cfg::flags {

enum Flags : uint64_t {
    // General perpose marker #1
    kMarker1 = 1ull,
    // General perpose marker #2
    kMarker2 = 1ull << 1,
    // General perpose marker #3
    kMarker3 = 1ull << 2,

    // Indicates that this bb should be ignored
    kIgnored = 1ull << 3,
};

} // namespace jit_aot::flags

#endif // COMMON_FLAGS_HPP
