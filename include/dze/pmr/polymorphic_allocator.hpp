#pragma once

#include <memory_resource>

namespace dze::pmr {

class polymorphic_allocator
{
public:
    using value_type = std::byte;

    polymorphic_allocator() = default;

    polymorphic_allocator(std::pmr::memory_resource* const r)
        : m_underlying{r} {}

    [[nodiscard]] void* allocate_bytes(
        const size_t n, const size_t alignment = alignof(std::max_align_t))
    {
        return m_underlying.resource()->allocate(n, alignment);
    }

    void deallocate_bytes(
        void* const p, const size_t n, const size_t alignment = alignof(std::max_align_t))
    {
        m_underlying.resource()->deallocate(p, n, alignment);
    }

private:
    std::pmr::polymorphic_allocator<std::byte> m_underlying;

    [[nodiscard]] friend bool operator==(
        const polymorphic_allocator& lhs, const polymorphic_allocator& rhs) noexcept
    {
        return lhs.m_underlying == rhs.m_underlying;
    }

    [[nodiscard]] friend bool operator!=(
        const polymorphic_allocator& lhs, const polymorphic_allocator& rhs) noexcept
    {
        return lhs.m_underlying != rhs.m_underlying;
    }
};

} // namespace dze::pmr
