#ifndef DFS_HPP
#define DFS_HPP

#include <jit_aot/ir/basic_block.hpp>

#include "common_flags.hpp"

namespace jit_aot::cfg {

namespace _dfs_detail {

// Do DFS algorithm. Flags should be cleared before this function call.
// Sets bbs flags, so those should be reset
void getDfsOrderHelper(ir::BasicBlock *root,
                       std::vector<ir::BasicBlock *> &out) {
    JA_ENSHURE(root != nullptr);

    if (root->testFlags(flags::kIgnored)) {
        return;
    }

    root->addFlags(flags::kMarker);
    out.push_back(root);
    for (auto it = root->succBegin(), end = root->succEnd(); it != end; ++it) {
        if (!(*it)->testFlags(flags::kMarker)) {
            getDfsOrderHelper(*it, out);
        }
    }
}

} // namespace _dfs_detail

// Get DFS order for graph with given root.
// Flags should be cleared before this function call
std::vector<ir::BasicBlock *> getDfsOrder(ir::BasicBlock *root) {
    std::vector<ir::BasicBlock *> out{};

    _dfs_detail::getDfsOrderHelper(root, out);

    for (auto bb : out) {
        bb->resetFlags();
    }

    return out;
}

} // namespace jit_aot::cfg

#endif // DFS_HPP
