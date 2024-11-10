#ifndef RPO_HPP
#define RPO_HPP

#include <jit_aot/ir/basic_block.hpp>

namespace jit_aot::cfg {

namespace _rpo_detail {

constexpr uint64_t kMarker = 1ull << 63;

// Do RPO algorithm. Flags should be cleared before this function call.
// Sets bbs flags, so those should be reset
void getRpoOrderHelper(ir::BasicBlock *root, std::vector<ir::BasicBlock *> &out,
                       int &bb_count) {
    JA_ENSHURE(root != nullptr);

    root->setFlags(kMarker);

    for (auto it = root->succBegin(), end = root->succEnd(); it != end; ++it) {
        if (!(*it)->testFlags(kMarker)) {
            getRpoOrderHelper(*it, out, bb_count);
        }
    }

    JA_ENSHURE(bb_count > 0);
    out[--bb_count] = root;
}

} // namespace _rpo_detail

// Get RPO order for graph with given root.
// Flags should be cleared before this function call
std::vector<ir::BasicBlock *> getRpoOrder(ir::BasicBlock *root, int bb_count) {
    std::vector<ir::BasicBlock *> out(bb_count, nullptr);

    _rpo_detail::getRpoOrderHelper(root, out, bb_count);

    for (auto bb : out) {
        bb->resetFlags();
    }

    return out;
}

} // namespace jit_aot::cfg

#endif // RPO_HPP
