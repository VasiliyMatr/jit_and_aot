#ifndef ARITH_HPP
#define ARITH_HPP

#include <jit_aot/peephole.hpp>

namespace jit_aot::peephole {

struct SubPeephole final : public IPeephole {
    std::pair<bool, ir::BasicBlock::Instrs::iterator>
    apply(ir::BasicBlock::Instrs::iterator it, ir::BasicBlock &bb) override;
};

struct ShlPeephole final : public IPeephole {
    std::pair<bool, ir::BasicBlock::Instrs::iterator>
    apply(ir::BasicBlock::Instrs::iterator it, ir::BasicBlock &bb) override;
};

struct OrPeephole final : public IPeephole {
    std::pair<bool, ir::BasicBlock::Instrs::iterator>
    apply(ir::BasicBlock::Instrs::iterator it, ir::BasicBlock &bb) override;
};


} // namespace jit_aot::peephole

#endif // ARITH_HPP
