#ifndef DOM_HPP
#define DOM_HPP

#include <sstream>

#include <jit_aot/ir/basic_block.hpp>

#include "dfs.hpp"

namespace jit_aot::cfg {

// [bb -> dominators]
using DomMap =
    std::unordered_map<ir::BasicBlock *, std::vector<ir::BasicBlock *>>;

DomMap getDomMap(ir::BasicBlock *root) {
    DomMap out{};

    auto dfs_order = getDfsOrder(root);

    for (auto dom : dfs_order) {
        out.try_emplace(dom); // create entry for each bb

        dom->setFlags(flags::kIgnored);
        auto reachable = getDfsOrder(root);
        dom->resetFlags();

        std::vector<ir::BasicBlock *> dominated{};
        for (auto bb : dfs_order) {
            if (bb != dom && std::find(reachable.begin(), reachable.end(),
                                       bb) == reachable.end()) {
                dominated.push_back(bb);
            }
        }

        for (auto bb : dominated) {
            out[bb].push_back(dom);
        }
    }

    return out;
}

// [bb -> imm dominator]
using ImmDomMap = std::unordered_map<ir::BasicBlock *, ir::BasicBlock *>;

ImmDomMap getImmDoms(const DomMap &dom_map) {
    ImmDomMap out{};

    for (const auto &[bb, doms] : dom_map) {
        auto doms_num = doms.size();

        if (doms_num == 0) {
            out[bb] = nullptr;
            continue;
        }

        // Search for dominator that is dominated by all other dominators
        auto it = std::find_if(
            doms.cbegin(), doms.cend(), [&](ir::BasicBlock *dom) -> bool {
                return dom_map.at(dom).size() + 1 == doms_num;
            });

        JA_ENSHURE(it != doms.cend());

        out[bb] = *it;
    }

    return out;
}

ImmDomMap getImmDoms(ir::BasicBlock *root) {
    auto dom_map = getDomMap(root);
    return getImmDoms(dom_map);
}

std::string dumpImmDomMap(const ImmDomMap &map) {
    std::stringstream out;

    out << "digraph CFG {\n";
    out << "node [shape = rectangle]\n\n";

    for (const auto &[bb, dom] : map) {
        const void *bb_v = bb;
        const void *dom_v = dom;

        if (dom != nullptr) {
            out << fmt::format("bb_{} -> bb_{}\n", dom_v, bb_v);
        }

        out << fmt::format("bb_{}[label = \"{}\"]\n", bb_v, bb->name());
    }

    out << "\n}\n";

    return out.str();
}

} // namespace jit_aot::cfg

#endif // DOM_HPP
