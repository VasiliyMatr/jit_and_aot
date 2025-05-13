#ifndef CHECK_ELIMINATION_HPP
#define CHECK_ELIMINATION_HPP

#include <jit_aot/cfg_analyze/dom.hpp>
#include <jit_aot/cfg_analyze/rpo.hpp>
#include <jit_aot/ir/module.hpp>

namespace jit_aot::check_elimination {

bool areSimilarChecks(const ir::instr::Instr *check,
                      const ir::instr::Instr *other) {
    if (check->type() != other->type()) {
        return false;
    }

    if (check->type() == ir::instr::InstrType::kNullCheck) {
        return check->getNthOpId(0) == other->getNthOpId(0);
    }

    if (check->type() == ir::instr::InstrType::kBoundCheck) {
        return check->getNthOpId(0) == other->getNthOpId(0) &&
               check->getNthOpId(1) == other->getNthOpId(1);
    }

    throw std::runtime_error("Unknown check type");
}

void eliminateSimilarChecksFromBb(const ir::instr::Instr *dominator,
                                  ir::BasicBlock *dominated,
                                  ir::BasicBlock::Instrs::iterator begin) {
    for (auto i = begin, end = dominated->end(); i != end;) {
        if (areSimilarChecks(dominator, i->get())) {
            dominated->removeInstr(i++);
        } else {
            ++i;
        }
    }
}

void eliminateChecks(ir::Function &fn) {
    auto rev_dom_map = cfg::getRevDomMap(fn.entry());
    auto rpo = cfg::getRpoOrder(fn.entry(), fn.getBbNum());

    for (auto bb : rpo) {
        for (auto i = bb->begin(), end = bb->end(); i != end; ++i) {
            const auto *instr = i->get();
            if (isCheck(instr->type())) {
                eliminateSimilarChecksFromBb(instr, bb, std::next(i));

                for (auto dominated : rev_dom_map[bb]) {
                    eliminateSimilarChecksFromBb(instr, dominated,
                                                 dominated->begin());
                }
            }
        }
    }
}

} // namespace jit_aot::check_elimination

#endif // CHECK_ELIMINATION_HPP
