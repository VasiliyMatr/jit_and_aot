#ifndef VALUES_HPP
#define VALUES_HPP

#include <variant>

#include <jit_aot/ir/types.hpp>

namespace jit_aot::ir {

namespace instr {
class Instr;
} // namespace instr

using ValueId = int;
constexpr ValueId kInvalidValueId = -1;

class Value {
  protected:
    ValueId m_id = kInvalidValueId;
    instr::Instr *m_producer = nullptr;

    Value() = default;

    Value(ValueId id, instr::Instr *producer)
        : m_id(id), m_producer(producer) {}

  public:
    ~Value() = default;

    JA_NODISCARD auto id() const noexcept { return m_id; }
    JA_NODISCARD auto *producer() const noexcept { return m_producer; }

    JA_NODISCARD virtual const InterfaceValueType &type() const = 0;
    JA_NODISCARD virtual fmt_it format(fmt_it out) const {
        out = type().format(out);
        return fmt::format_to(out, " %{}", m_id);
    }
};

class IntValue : public Value {
    IntType m_type;

  public:
    IntValue(IntType type, ValueId id, instr::Instr *producer)
        : Value(id, producer), m_type(type) {}

    JA_NODISCARD const IntType &type() const override { return m_type; }
};

class IntConstant final : public IntValue {
    uint64_t m_value = 0;

  public:
    IntConstant(IntType type, uint64_t value)
        : IntValue(type, kInvalidValueId, nullptr), m_value(value) {}

    JA_NODISCARD auto value() const noexcept { return m_value; }

    JA_NODISCARD fmt_it format(fmt_it out) const override {
        out = type().format(out);
        return fmt::format_to(out, " {}", m_value);
    }
};

struct IntFuncArg final : public IntValue {
    IntFuncArg(IntType type, ValueId id) : IntValue(type, id, nullptr) {}
};

} // namespace jit_aot::ir

#endif // VALUES_HPP
