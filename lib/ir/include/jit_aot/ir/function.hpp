#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <vector>

#include <jit_aot/ir/basic_block.hpp>

namespace jit_aot::ir {

class Function final {
    using BasicBlocks = std::vector<std::unique_ptr<BasicBlock>>;

    std::string m_name = "";
    BasicBlocks m_bbs{};
    BasicBlock *m_entry = nullptr;

    IntType m_ret_type;
    std::vector<IntArgValue> m_args{};

    size_t m_next_id = 0;

  public:
    // Get next function entity id
    auto nextId() noexcept { return m_next_id++; }

    Function(IntType ret_type, std::vector<IntType> args_types)
        : m_ret_type(ret_type) {
        m_args.reserve(args_types.size());

        for (auto &&arg_type : args_types) {
            m_args.push_back({arg_type, nextId()});
        }
    }

    void setName(std::string name) noexcept { m_name = std::move(name); }
    JA_NODISCARD std::string_view name() const noexcept { return m_name; }

    JA_NODISCARD auto retType() const noexcept { return m_ret_type; }
    JA_NODISCARD auto &args() const noexcept { return m_args; }

    JA_NODISCARD auto begin() noexcept { return m_bbs.begin(); }
    JA_NODISCARD auto end() noexcept { return m_bbs.end(); }

    JA_NODISCARD auto begin() const noexcept { return m_bbs.cbegin(); }
    JA_NODISCARD auto end() const noexcept { return m_bbs.cend(); }

    void addBb(std::unique_ptr<BasicBlock> bb, bool entry = false) {
        m_bbs.push_back(std::move(bb));
        if (entry) {
            JA_ENSHURE(m_entry == nullptr);
            m_entry = m_bbs.back().get();
        }
    }

    JA_NODISCARD auto *entry() const noexcept { return m_entry; }
};

} // namespace jit_aot::ir

#endif // FUNCTION_HPP
