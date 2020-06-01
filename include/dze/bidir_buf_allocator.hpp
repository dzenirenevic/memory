#pragma once

#include <cstddef>
#include <cstdlib>
#include <type_traits>

#include "details/bidir_allocate.hpp"

namespace dze {

class bidir_buf_allocator
{
public:
    using value_type = std::byte;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::false_type;

    constexpr bidir_buf_allocator(void* const buf, const size_t size) noexcept
        : m_buf{buf}
        , m_size{size} {}

    [[nodiscard]] void* allocate_bytes(const size_t n, const size_t alignment = alignof(std::max_align_t))
    {
        return details::memory::bidir_allocate(m_buf, m_size, n, alignment);
    }

    // Undefined behavior if p is not equal to the return value of the last invocation of
    // allocate_bytes on this object.
    void deallocate_bytes(
        void* const p, const size_t n, const size_t alignment = alignof(std::max_align_t)) noexcept
    {
        details::memory::bidir_deallocate(m_buf, m_size, p, n, alignment);
    }

private:
    void* m_buf;
    size_t m_size;

    [[nodiscard]] friend constexpr bool operator==(
        const bidir_buf_allocator& lhs, const bidir_buf_allocator& rhs) noexcept
    {
        return lhs.m_buf == rhs.m_buf && lhs.m_size == rhs.m_size;
    }

    [[nodiscard]] friend constexpr bool operator!=(
        const bidir_buf_allocator& lhs, const bidir_buf_allocator& rhs) noexcept
    {
        return !(lhs == rhs);
    }
};

} // namespace dze
