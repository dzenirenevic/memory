#pragma once

#include <cstddef>

#include "resource.hpp"

namespace dze::pmr {

class polymorphic_allocator
{
public:
    using value_type = std::byte;

    polymorphic_allocator() noexcept
        : polymorphic_allocator{std::pmr::get_default_resource()} {}

    polymorphic_allocator(std::pmr::memory_resource* const r) noexcept
        : m_resource{r} {}

    // NOLINTNEXTLINE(readability-make-member-function-const)
    [[nodiscard]] void* allocate_bytes(
        const size_t n, const size_t alignment = alignof(std::max_align_t))
    {
        return resource()->allocate(n, alignment);
    }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    [[nodiscard]] void* reallocate_bytes(
        void* const p,
        const size_t n,
        const size_t new_size,
        const size_t alignment = alignof(std::max_align_t))
    {
        return static_cast<memory_resource*>(resource())->reallocate(p, n, new_size, alignment);
    }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    void deallocate_bytes(
        void* const p, const size_t n, const size_t alignment = alignof(std::max_align_t))
    {
        resource()->deallocate(p, n, alignment);
    }

    [[nodiscard]] std::pmr::memory_resource* resource() const noexcept { return m_resource; }

private:
    std::pmr::memory_resource* m_resource;

    [[nodiscard]] friend bool operator==(
        const polymorphic_allocator& lhs, const polymorphic_allocator& rhs) noexcept
    {
        return lhs.resource()->is_equal(*rhs.resource());
    }

    [[nodiscard]] friend bool operator!=(
        const polymorphic_allocator& lhs, const polymorphic_allocator& rhs) noexcept
    {
        return !(lhs == rhs);
    }
};

} // namespace dze::pmr
