#pragma once

#include <cstddef>
#include <cstring>
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
    [[nodiscard]] void* reallocate_bytes(
        void* const p,
        const size_t n,
        const size_t new_size,
        const size_t alignment = alignof(std::max_align_t)) const
    {
        void* bytes;
        if (alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
        {
            bytes = ::operator new(new_size, std::align_val_t{alignment});
            std::memcpy(bytes, p, n);
            ::operator delete(p, n, std::align_val_t{alignment});
        }
        else
        {
            bytes = ::operator new(new_size);
            std::memcpy(bytes, p, n);
            ::operator delete(p, n);
        }
        return bytes;
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void deallocate_bytes(
        void* const p,
        const size_t n,
        const size_t alignment = alignof(std::max_align_t)) const noexcept
    {
        if (alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
            ::operator delete(p, n, std::align_val_t{alignment});
        else
            ::operator delete(p, n);
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
