#ifndef FORMATTING_HPP
#define FORMATTING_HPP

#include <fmt/format.h>

#include <jit_aot/ir/function.hpp>

/// Value types formatting

template <>
struct fmt::formatter<jit_aot::ir::InterfaceValueType>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::InterfaceValueType &value_type,
                fmt::format_context &ctx) const {
        return value_type.format(ctx.out());
    }
};

template <>
struct fmt::formatter<jit_aot::ir::IntType>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::IntType &int_type,
                fmt::format_context &ctx) const {
        return int_type.format(ctx.out());
    }
};

/// Values formatting

template <>
struct fmt::formatter<jit_aot::ir::Value>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::Value &value,
                fmt::format_context &ctx) const {
        return value.format(ctx.out());
    }
};

template <>
struct fmt::formatter<jit_aot::ir::IntValue>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::IntValue &value,
                fmt::format_context &ctx) const {
        return value.format(ctx.out());
    }
};

template <>
struct fmt::formatter<jit_aot::ir::IntFuncArg>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::IntFuncArg &arg,
                fmt::format_context &ctx) const {
        return arg.format(ctx.out());
    }
};

/// Instructions formatting

template <>
struct fmt::formatter<jit_aot::ir::instr::InstrType>
    : public fmt::formatter<string_view> {
    auto format(const jit_aot::ir::instr::InstrType &type,
                fmt::format_context &ctx) const {
        string_view name = "";

        switch (type) {
        case jit_aot::ir::instr::InstrType::kIntBinArithAdd:
            name = "add";
            break;
        case jit_aot::ir::instr::InstrType::kIntBinArithSub:
            name = "sub";
            break;
        case jit_aot::ir::instr::InstrType::kIntBinArithMul:
            name = "mul";
            break;
        case jit_aot::ir::instr::InstrType::kIntBinArithDiv:
            name = "div";
            break;
        case jit_aot::ir::instr::InstrType::kIntBinCmpEq:
            name = "cmp.eq";
            break;
        case jit_aot::ir::instr::InstrType::kIntBinCmpLl:
            name = "cmp.ll";
            break;
        case jit_aot::ir::instr::InstrType::kIntBinCmpGg:
            name = "cmp.gg";
            break;
        case jit_aot::ir::instr::InstrType::kIntPhiNode:
            name = "phi";
            break;
        case jit_aot::ir::instr::InstrType::kBrDirectUncond:
            name = "b";
            break;
        case jit_aot::ir::instr::InstrType::kBrDirectCond:
            name = "b.if";
            break;
        case jit_aot::ir::instr::InstrType::kReturn:
            name = "return";
            break;

        default:
            name = "[undef instr type]";
            break;
        }

        return formatter<string_view>::format(name, ctx);
    }
};

template <>
struct fmt::formatter<jit_aot::ir::instr::Instr>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::instr::Instr &instr,
                fmt::format_context &ctx) const {
        return instr.format(ctx.out());
    }
};

namespace jit_aot::ir {

JA_NODISCARD inline fmt_it instr::Instr::format(fmt_it out) const {
    if (const auto *res = result()) {
        out = fmt::format_to(out, "{} = ", *res);
    }

    out = fmt::format_to(out, "{}", type());

    bool first = true;
    for (auto it = opBegin(), end = opEnd(); it != end; ++it) {
        out = fmt::format_to(out, "{}{}", first ? " " : ", ", **it);
        first = false;
    }

    return out;
}

JA_NODISCARD inline fmt_it instr::IntPhiNode::format(fmt_it out) const {
    out = fmt::format_to(out, "{} = {}", *result(), type());

    bool first = true;
    for (auto [bb, value] : m_map) {
        out = fmt::format_to(out, "{}[{} -> {}]", first ? " " : ", ",
                             bb->name(), *value);
        first = false;
    }

    return out;
}

JA_NODISCARD inline fmt_it instr::BrDirectUncond::format(fmt_it out) const {
    return fmt::format_to(out, "{} {}", type(), dest->name());
}

JA_NODISCARD inline fmt_it instr::BrDirectCond::format(fmt_it out) const {
    return fmt::format_to(out, "{} {} {}, else {}", type(), **opBegin(),
                          dest->name(), fallthrough->name());
}

} // namespace jit_aot::ir

/// Other formatters

template <>
struct fmt::formatter<jit_aot::ir::BasicBlock>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::BasicBlock &bb,
                fmt::format_context &ctx) const {
        auto out = fmt::format_to(ctx.out(), "{}:\n", bb.name());

        for (auto &&instr : bb) {
            out = fmt::format_to(out, "  {}\n", *instr);
        }

        return out;
    }
};

template <>
struct fmt::formatter<jit_aot::ir::Function>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::Function &func,
                fmt::format_context &ctx) const {
        auto out =
            fmt::format_to(ctx.out(), "{} {} (", func.retType(), func.name());

        bool first = true;
        for (auto arg_it = func.argsBegin(), end = func.argsEnd();
             arg_it != end; ++arg_it) {
            out = fmt::format_to(out, "{}{}", first ? "" : ", ", *arg_it);
            first = false;
        }

        out = fmt::format_to(out, ")\n");

        for (const auto &bb : func) {
            out = fmt::format_to(out, "{}{}\n",
                                 &bb == func.entry() ? "[entry] " : "", bb);
        }

        return out;
    }
};

#endif // FORMATTING_HPP
