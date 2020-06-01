#pragma once

#include <cstddef>
#include <new>
#include <type_traits>

#include <mimalloc.h>

namespace dze {

class miallocator
{
public:
    using value_type = std::byte;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    [[nodiscard]] void* allocate_bytes(
        const size_t n, const size_t alignment = alignof(std::max_align_t)) const
    {
        void* bytes;
        if (alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
            bytes = mi_malloc_aligned(n, alignment);
        else
            bytes = mi_malloc(n);

        if (bytes == nullptr)
            throw std::bad_alloc{};

        return bytes;
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void deallocate_bytes(
        void* const p, size_t, size_t = alignof(std::max_align_t)) const noexcept
    {
        mi_free(p);
    }

private:
    [[nodiscard]] friend constexpr bool operator==(miallocator, miallocator) noexcept
    {
        return true;
    }

    [[nodiscard]] friend constexpr bool operator!=(miallocator, miallocator) noexcept
    {
        return false;
    }
};

} // namespace dze
