#ifndef LOOP_TREE_HPP
#define LOOP_TREE_HPP

#include "dom.hpp"
#include "rpo.hpp"

namespace jit_aot::cfg {

struct Loop final {
    ir::BasicBlock *head = nullptr;
    std::vector<ir::BasicBlock *> back_edges{};
    // All basic blocks
    std::vector<ir::BasicBlock *> bbs{};

    bool reducible = true;

    Loop *parent = nullptr;
    std::vector<Loop *> children{};
};

// [head -> loop info]
using Loops = std::unordered_map<ir::BasicBlock *, Loop>;

namespace _loop_detail {

// Run DFS search to fill Loops map.
// Following fields are filled after this pass: Loop::head, Loop::back_edges,
// Loop::reducible
void collectBackEdges(ir::BasicBlock *root, Loops &out, const DomMap &dom_map) {
    JA_ENSHURE(root != nullptr);

    root->setFlags(flags::kMarker1 | flags::kMarker2);

    for (auto it = root->succBegin(), end = root->succEnd(); it != end; ++it) {
        auto succ = *it;

        // Loop found
        if (succ->testFlags(flags::kMarker2)) {
            auto &loop = out[succ];
            JA_ENSHURE(loop.head == nullptr || loop.head == succ);

            loop.head = succ;
            loop.bbs.push_back(succ);
            loop.back_edges.push_back(root);

            // Check for irreducible loop
            const auto &back_edge_dom = dom_map.at(root);
            if (std::find(back_edge_dom.begin(), back_edge_dom.end(), succ) ==
                back_edge_dom.end()) {
                loop.reducible = false;
            }
        }

        // Continue DFS
        if (!succ->testFlags(flags::kMarker1)) {
            collectBackEdges(*it, out, dom_map);
        }
    }

    root->setFlags(flags::kMarker1); // unmark kMarker2
}

using BbToLoop = std::unordered_map<ir::BasicBlock *, Loop *>;

// Run loop search for given root and loop
void loopSearch(ir::BasicBlock *root, BbToLoop &bb_to_loop, Loop *loop) {
    if (root->testFlags(flags::kMarker1)) {
        return;
    }

    root->addFlags(flags::kMarker1);
    auto &root_loop = bb_to_loop[root];

    // Block without a loop
    if (root_loop == nullptr) {
        root_loop = loop;
        loop->bbs.push_back(root);
    }
    // Block with other loop (inner)
    else if (root_loop->parent == nullptr) {
        loop->children.push_back(root_loop);
        root_loop->parent = loop;
    }

    // Run dfs for all preds
    for (auto it = root->predBegin(), end = root->predEnd(); it != end; ++it) {
        if (!(*it)->testFlags(flags::kMarker1)) {
            loopSearch(*it, bb_to_loop, loop);
        }
    }
}

// Populate loops: fill `bbs` fields and `BbToLoop` map
void populateLoops(const std::vector<ir::BasicBlock *> &rpo, Loops &out,
                   BbToLoop &bb_to_loop) {

    // Traverse all loop headers in reverse rpo order
    for (auto it = rpo.rbegin(), end = rpo.rend(); it != end; ++it) {
        auto bb = *it;

        auto loop_it = out.find(bb);
        if (loop_it == out.end()) {
            continue;
        }
        auto &loop = loop_it->second;

        // Add root to BbToLoop map
        bb_to_loop[bb] = &loop;

        // For irreducible loops - append all source of back edges in loop
        if (!loop.reducible) {
            for (auto back_edge : loop.back_edges) {
                loop.bbs.push_back(back_edge);
                bb_to_loop[back_edge] = &loop;
            }
            continue;
        }

        // For reducible loops - for all back edges run loop search
        loop.head->addFlags(flags::kMarker1);
        for (auto back_edge : loop.back_edges) {
            loopSearch(back_edge, bb_to_loop, &loop);
        }

        // Reset flags after dfs
        for (auto bb : rpo) {
            bb->resetFlags();
        }
    }
}

} // namespace _loop_detail

// Get loop tree for given root bb
Loops getLoopTree(ir::BasicBlock *root) {
    // [1] Get dom tree
    auto dom_map = getDomMap(root);
    auto imm_dom_map = getImmDoms(dom_map);
    auto rpo = getRpoOrder(root, static_cast<int>(dom_map.size()));

    // [2] Collect back edges
    Loops loops{};
    _loop_detail::collectBackEdges(root, loops, dom_map);

    // Reset flags after dfs
    for (auto bb : rpo) {
        bb->resetFlags();
    }

    // [3] Populate loops
    _loop_detail::BbToLoop bb_to_loop{};
    _loop_detail::populateLoops(rpo, loops, bb_to_loop);

    // [4] Build outer loop
    auto &root_loop = loops[nullptr];
    root_loop.reducible = false;

    // Insert all blocks wthout loops into root loop
    for (auto bb : rpo) {
        auto &bb_loop = bb_to_loop[bb];
        if (bb_loop == nullptr) {
            bb_loop = &root_loop;
            root_loop.bbs.push_back(bb);
        }
    }

    // Attach all loops without parent to root loop
    for (auto &[loop_root, loop] : loops) {
        // Skip root loop
        if (loop_root == nullptr) {
            continue;
        }

        if (loop.parent == nullptr) {
            loop.parent = &root_loop;
            root_loop.children.push_back(&loop);
        }
    }

    return loops;
}

// Dump given loop tree into dot format
void dumpLoopsToDot(std::ostream &out, const Loops &loops) {
    out << fmt::format("digraph LoopTree {{\n");
    out << fmt::format("node [shape = rectangle]\n\n");

    for (auto &[_, loop] : loops) {
        auto *head_v = static_cast<void *>(loop.head);

        // Dump node with all bbs
        out << fmt::format("loop_{} [label = \"", head_v);
        out << fmt::format("head: {}\\n",
                           loop.head ? loop.head->name() : "[null]");
        out << fmt::format("reducible: {}\\n", loop.reducible);
        out << "bbs:";

        for (auto bb : loop.bbs) {
            out << fmt::format(" {}", bb->name());
        }

        out << "\"]\n";

        // Dump loop tree edges
        for (auto child : loop.children) {
            out << fmt::format("loop_{} -> loop_{}\n", head_v,
                               static_cast<void *>(child->head));
        }
    }

    out << "}\n";
}

} // namespace jit_aot::cfg

#endif // LOOP_TREE_HPP
