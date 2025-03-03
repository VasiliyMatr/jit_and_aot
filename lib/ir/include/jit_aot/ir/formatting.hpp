#ifndef FORMATTING_HPP
#define FORMATTING_HPP

#include <fmt/format.h>

#include <jit_aot/ir/function.hpp>

/// Value types formatting

template <>
struct fmt::formatter<jit_aot::ir::InterfaceValueType>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::InterfaceValueType &value_type,
                format_context &ctx) const {
        return value_type.format(ctx.out());
    }
};

template <>
struct fmt::formatter<jit_aot::ir::IntType>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::IntType &int_type,
                format_context &ctx) const {
        return int_type.format(ctx.out());
    }
};

/// Values formatting

template <>
struct fmt::formatter<jit_aot::ir::Value>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::Value &value, format_context &ctx) const {
        return value.format(ctx.out());
    }
};

template <>
struct fmt::formatter<jit_aot::ir::IntValue>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::IntValue &value, format_context &ctx) const {
        return value.format(ctx.out());
    }
};

template <>
struct fmt::formatter<jit_aot::ir::IntFuncArg>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::IntFuncArg &arg, format_context &ctx) const {
        return arg.format(ctx.out());
    }
};

/// Instructions formatting

template <>
struct fmt::formatter<jit_aot::ir::instr::InstrType>
    : public fmt::formatter<string_view> {
    auto format(const jit_aot::ir::instr::InstrType &type,
                format_context &ctx) const {
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
        case jit_aot::ir::instr::InstrType::kIntBinArithShl:
            name = "shl";
            break;
        case jit_aot::ir::instr::InstrType::kIntBinArithOr:
            name = "or";
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
                format_context &ctx) const {
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

/// Other formatters

template <class T> struct IrFormatter {
    enum class FormatMode {
        kDefault,
        kDot,
        kInvalid,
    };

    FormatMode fmt_mode = FormatMode::kDefault;

    constexpr auto parse(fmt::format_parse_context &ctx) {
        auto it = ctx.begin();
        if (it == nullptr || *it == '}') {
            fmt_mode = FormatMode::kDefault;
            return it;
        }

        bool is_dot =
            *(it++) == 'd' && *(it++) == 'o' && *(it++) == 't' && *(it) == '}';

        if (is_dot) {
            fmt_mode = FormatMode::kDot;
            return it;
        }

        return ctx.begin();
    }

    virtual fmt_it format_default(const T &, fmt::format_context &) const = 0;
    virtual fmt_it format_dot(const T &, fmt::format_context &) const = 0;

    auto format(const T &bb, fmt::format_context &ctx) const {
        switch (fmt_mode) {
        case FormatMode::kDefault:
            return format_default(bb, ctx);

        case FormatMode::kDot:
            return format_dot(bb, ctx);

        default:
            JA_ENSHURE(0);
        }
    }
};

} // namespace jit_aot::ir

template <>
struct fmt::formatter<jit_aot::ir::BasicBlock>
    : public jit_aot::ir::IrFormatter<jit_aot::ir::BasicBlock> {

    format_context::iterator
    format_default(const jit_aot::ir::BasicBlock &bb,
                   format_context &ctx) const override {
        auto out = fmt::format_to(ctx.out(), "{}:\n", bb.name());

        for (auto &&instr : bb) {
            out = fmt::format_to(out, "  {}\n", *instr);
        }

        return out;
    }

    format_context::iterator format_dot(const jit_aot::ir::BasicBlock &bb,
                                        format_context &ctx) const override {
        const void *bb_ptr = &bb;

        auto out = fmt::format_to(ctx.out(), "bb_{} [label = \"{}\"]\n", bb_ptr,
                                  bb.name());

        for (auto succ_it = bb.succBegin(), succ_end = bb.succEnd();
             succ_it != succ_end; ++succ_it) {

            const void *succ_ptr = *succ_it;
            out = fmt::format_to(out, "bb_{} -> bb_{}\n", bb_ptr, succ_ptr);
        }

        return out;
    }
};

template <>
struct fmt::formatter<jit_aot::ir::Function>
    : public jit_aot::ir::IrFormatter<jit_aot::ir::Function> {
    fmt::format_context::iterator
    format_default(const jit_aot::ir::Function &func,
                   fmt::format_context &ctx) const override {
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

    fmt::format_context::iterator
    format_dot(const jit_aot::ir::Function &func,
               fmt::format_context &ctx) const override {
        auto out = fmt::format_to(ctx.out(), "digraph CFG {{\n");
        out = fmt::format_to(out, "node [shape = rectangle]\n\n");

        for (const auto &bb : func) {
            out = fmt::format_to(out, "{:dot}", bb);
        }

        out = fmt::format_to(out, "\n}}\n");

        return out;
    }
};

#endif // FORMATTING_HPP
