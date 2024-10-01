#ifndef FORMATTING_HPP
#define FORMATTING_HPP

#include <fmt/format.h>

#include <jit_aot/ir/function.hpp>

/// Types formatting

template <>
struct fmt::formatter<jit_aot::ir::IntType>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::IntType &int_type,
                fmt::format_context &ctx) const {
        return fmt::format_to(ctx.out(), "i{}", int_type.size);
    }
};

/// Values formatting

template <>
struct fmt::formatter<jit_aot::ir::IntConstant>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::IntConstant &val,
                fmt::format_context &ctx) const {
        return fmt::format_to(ctx.out(), "{} {}", val.type, val.value);
    }
};

template <>
struct fmt::formatter<jit_aot::ir::IntProduct>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::IntProduct &val,
                fmt::format_context &ctx) const {
        return fmt::format_to(ctx.out(), "{} %{}", val.type, val.id);
    }
};

template <>
struct fmt::formatter<jit_aot::ir::IntArgValue>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::IntArgValue &val,
                fmt::format_context &ctx) const {
        return fmt::format_to(ctx.out(), "{} %{}", val.type, val.id);
    }
};

template <>
struct fmt::formatter<jit_aot::ir::IntValue>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::IntValue &val,
                fmt::format_context &ctx) const {
        return std::visit(
            [&](const auto &val) -> auto{
                return fmt::format_to(ctx.out(), "{}", val);
            },
            val);
    }
};

/// Instructions formatting

template <>
struct fmt::formatter<jit_aot::ir::instr::BinaryInstrType>
    : public fmt::formatter<string_view> {
    auto format(const jit_aot::ir::instr::BinaryInstrType &type,
                fmt::format_context &ctx) const {
        string_view name = "";

        switch (type) {
        case jit_aot::ir::instr::BinaryInstrType::Add:
            name = "add";
            break;
        case jit_aot::ir::instr::BinaryInstrType::Sub:
            name = "sub";
            break;
        case jit_aot::ir::instr::BinaryInstrType::Mul:
            name = "mul";
            break;
        case jit_aot::ir::instr::BinaryInstrType::Div:
            name = "div";
            break;
        case jit_aot::ir::instr::BinaryInstrType::CmpEq:
            name = "cmp.eq";
            break;
        case jit_aot::ir::instr::BinaryInstrType::CmpLl:
            name = "cmp.ll";
            break;
        case jit_aot::ir::instr::BinaryInstrType::CmpGg:
            name = "cmp.gg";
            break;
        }

        return formatter<string_view>::format(name, ctx);
    }
};

template <>
struct fmt::formatter<jit_aot::ir::instr::IntBinaryInstr>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::instr::IntBinaryInstr &instr,
                fmt::format_context &ctx) const {
        return fmt::format_to(ctx.out(), "{} = {} {}, {}", instr.prod,
                              instr.type, instr.op1, instr.op2);
    }
};

template <>
struct fmt::formatter<jit_aot::ir::instr::IntPhiNode>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::instr::IntPhiNode &instr,
                fmt::format_context &ctx) const {
        auto out = fmt::format_to(ctx.out(), "{} = phi", instr.prod);

        bool first = true;
        for (auto &&option : instr.map) {
            out = fmt::format_to(out, "{} [{} -> {}]", first ? "" : ",",
                                 option.first->name(), option.second);
            first = false;
        }

        return out;
    }
};

template <>
struct fmt::formatter<jit_aot::ir::instr::DirectUncondBranch>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::instr::DirectUncondBranch &instr,
                fmt::format_context &ctx) const {
        return fmt::format_to(ctx.out(), "b {}", instr.dest->name());
    }
};

template <>
struct fmt::formatter<jit_aot::ir::instr::DirectCondBranch>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::instr::DirectCondBranch &instr,
                fmt::format_context &ctx) const {
        return fmt::format_to(ctx.out(), "b.if {} {}, else {}", instr.cond,
                              instr.dest->name(), instr.fallthrough->name());
    }
};

template <>
struct fmt::formatter<jit_aot::ir::instr::Return>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::instr::Return &instr,
                fmt::format_context &ctx) const {
        return fmt::format_to(ctx.out(), "ret {}", instr.value);
    }
};

/// Other formatters

template <>
struct fmt::formatter<jit_aot::ir::BasicBlock>
    : public jit_aot::IFmtNoParseFormatter {
    auto format(const jit_aot::ir::BasicBlock &bb,
                fmt::format_context &ctx) const {
        auto out = fmt::format_to(ctx.out(), "bb {}:\n", bb.name());

        for (auto &&instr : bb) {
            out = fmt::format_to(out, "  ");
            instr->format(out);
            *(out++) = '\n';
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
        for (auto &&arg : func.args()) {
            out = fmt::format_to(out, "{}{}", first ? "" : ", ", arg);
            first = false;
        }

        out = fmt::format_to(out, ")\n");

        for (auto &&bb : func) {
            out = fmt::format_to(out, "{}{}\n",
                                 bb.get() == func.entry() ? "entry " : "", *bb);
        }

        return out;
    }
};

#endif // FORMATTING_HPP
