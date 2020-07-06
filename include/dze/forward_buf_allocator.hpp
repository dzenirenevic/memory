#pragma once

#include <cstddef>
#include <cstdlib>
#include <type_traits>

#include "details/forward_allocate.hpp"

namespace dze {

class forward_buf_allocator
{
public:
    using value_type = std::byte;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::false_type;

    constexpr forward_buf_allocator(void* const buf, const size_t size) noexcept
        : m_buf{buf}
        , m_size{size} {}

    [[nodiscard]] void* allocate_bytes(size_t n, size_t alignment = alignof(std::max_align_t))
    {
        return details::memory::forward_allocate(m_buf, m_size, n, alignment);
    }

    [[nodiscard]] void* reallocate_bytes(
        void* const p,
        const size_t n,
        const size_t new_size,
        const size_t alignment = alignof(std::max_align_t))
    {
        return details::memory::forward_reallocate(m_buf, m_size, p, n, new_size, alignment);
    }

    constexpr void deallocate_bytes(
        void*, size_t, size_t = alignof(std::max_align_t)) const noexcept {}

private:
    void* m_buf;
    size_t m_size;

    [[nodiscard]] friend constexpr bool operator==(
        const forward_buf_allocator& lhs, const forward_buf_allocator& rhs) noexcept
    {
        return lhs.m_buf == rhs.m_buf && lhs.m_size == rhs.m_size;
    }

    [[nodiscard]] friend constexpr bool operator!=(
        const forward_buf_allocator& lhs, const forward_buf_allocator& rhs) noexcept
    {
        return !(lhs == rhs);
    }
};

} // namespace dze
