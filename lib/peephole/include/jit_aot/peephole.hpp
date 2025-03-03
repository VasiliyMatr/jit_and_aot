#ifndef PEEPHOLE_HPP
#define PEEPHOLE_HPP

#include <jit_aot/ir/module.hpp>

namespace jit_aot {

struct IPeephole {
    virtual ~IPeephole() = 0;

    virtual std::pair<bool, ir::BasicBlock::Instrs::iterator>
    apply(ir::BasicBlock::Instrs::iterator it, ir::BasicBlock &bb) = 0;
};

inline IPeephole::~IPeephole() = default;

template <class PeepholeIt>
void applyPeepholes(ir::Function &fn, PeepholeIt p_begin, PeepholeIt p_end,
                    size_t passes_limit) {
    bool applied = false;
    size_t passes_num = 0;

    do {
        applied = false;

        for (PeepholeIt p_it = p_begin; p_it != p_end; ++p_it) {
            for (auto &bb : fn) {
                auto bb_it = bb.begin();
                auto bb_end = bb.end();

                while (bb_it != bb_end) {
                    auto [curr_applied, it] = (*p_it)->apply(bb_it, bb);

                    if (curr_applied) {
                        applied = true;
                        bb_it = it;
                    } else {
                        ++bb_it;
                    }
                }
            }
        }
    } while (applied && passes_num++ < passes_limit);
}

} // namespace jit_aot

#endif // PEEPHOLE_HPP
