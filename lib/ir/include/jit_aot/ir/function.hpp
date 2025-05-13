#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <vector>

#include <jit_aot/ir/basic_block.hpp>

namespace jit_aot::ir {

class Module;

using FuncEntityId = int;
static_assert(std::is_same_v<FuncEntityId, ValueId>,
              "Values are indexed as functions entities");

class Function final {
    // Use list for fixed ptrs
    using BasicBlocks = std::list<BasicBlock>;

    std::string m_name = "";
    Module *m_module = nullptr;

    BasicBlocks m_bbs{};
    BasicBlock *m_entry = nullptr;

    const InterfaceValueType *m_ret_type = nullptr;
    std::vector<std::unique_ptr<Value>> m_args{};

    FuncEntityId m_next_id = 0;

  public:
    // Get next function entity id
    auto nextId() noexcept { return m_next_id++; }

    Function(Module *module, const InterfaceValueType *ret_type,
             std::vector<const InterfaceValueType *> args_types)
        : m_module(module), m_ret_type(ret_type) {
        m_args.reserve(args_types.size());

        for (auto &&arg_type : args_types) {
            if (auto int_ty = dynamic_cast<const IntType *>(arg_type)) {
                m_args.emplace_back(
                    std::make_unique<IntValue>(*int_ty, nextId(), nullptr));
                continue;
            }

            if (auto ptr_ty = dynamic_cast<const PointerType *>(arg_type)) {
                m_args.emplace_back(
                    std::make_unique<Pointer>(ptr_ty, nextId(), nullptr));
                continue;
            }

            throw std::runtime_error("Unknown argument type");
        }
    }

    void setName(std::string name) noexcept { m_name = std::move(name); }
    JA_NODISCARD std::string_view name() const noexcept { return m_name; }

    JA_NODISCARD auto *module() const noexcept { return m_module; }

    JA_NODISCARD const auto *retType() const noexcept { return m_ret_type; }
    JA_NODISCARD const auto *argsBegin() const noexcept {
        return m_args.data();
    }
    JA_NODISCARD const auto *argsEnd() const noexcept {
        return m_args.data() + m_args.size();
    }
    JA_NODISCARD const auto *nthArg(size_t n) const noexcept {
        return m_args[n].get();
    }
    template <class T> JA_NODISCARD const T *nthArgAs(size_t n) const noexcept {
        return dynamic_cast<const T *>(nthArg(n));
    }

    JA_NODISCARD auto begin() noexcept { return m_bbs.begin(); }
    JA_NODISCARD auto end() noexcept { return m_bbs.end(); }
    JA_NODISCARD auto begin() const noexcept { return m_bbs.cbegin(); }
    JA_NODISCARD auto end() const noexcept { return m_bbs.cend(); }

    JA_NODISCARD auto *getBb(std::string_view bb_name) {
        auto it =
            std::find_if(begin(), end(), [bb_name](BasicBlock &bb) -> bool {
                return bb.name() == bb_name;
            });

        return it != end() ? &*it : nullptr;
    }

    JA_NODISCARD auto getBbNum() const noexcept { return m_bbs.size(); }

    JA_NODISCARD auto *addBb(std::string bb_name, bool entry = false) {
        auto *new_bb = &m_bbs.emplace_back(this, std::move(bb_name));

        if (entry) {
            JA_ENSHURE(m_entry == nullptr);
            m_entry = new_bb;
        }

        return new_bb;
    }

    void removeBb(BasicBlocks::iterator it) { m_bbs.erase(it); }

    JA_NODISCARD auto *entry() const noexcept { return m_entry; }
};

} // namespace jit_aot::ir

#endif // FUNCTION_HPP
