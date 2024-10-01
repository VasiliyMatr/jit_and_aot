#include <jit_aot/ir/formatting.hpp>

namespace jit_aot::ir::instr {

fmt_it IntBinaryInstr::format(fmt_it out) {
    return fmt::format_to(out, "{}", *this);
}

fmt_it IntPhiNode::format(fmt_it out) {
    return fmt::format_to(out, "{}", *this);
}

fmt_it DirectUncondBranch::format(fmt_it out) {
    return fmt::format_to(out, "{}", *this);
}

fmt_it DirectCondBranch::format(fmt_it out) {
    return fmt::format_to(out, "{}", *this);
}

fmt_it Return::format(fmt_it out) { return fmt::format_to(out, "{}", *this); }

} // namespace jit_aot::ir::instr
