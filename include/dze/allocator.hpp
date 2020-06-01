#pragma once

#include <cstddef>
#include <new>
#include <type_traits>

namespace dze {

class allocator
{
public:
    using value_type = std::byte;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    [[nodiscard]] void* allocate_bytes(
        const size_t n, const size_t alignment = alignof(std::max_align_t)) const
    {
        if (alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
            return ::operator new(n, std::align_val_t{alignment});
        else
            return ::operator new(n);
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void deallocate_bytes(
        void* const p, const size_t size, const size_t alignment = alignof(std::max_align_t)) const noexcept
    {
        if (alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
            ::operator delete(p, size, std::align_val_t{alignment});
        else
            ::operator delete(p, size);
    }

private:
    [[nodiscard]] friend constexpr bool operator==(allocator, allocator) noexcept
    {
        return true;
    }

    [[nodiscard]] friend constexpr bool operator!=(allocator, allocator) noexcept
    {
        return false;
    }
};

} // namespace dze
