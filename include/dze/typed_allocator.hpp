#pragma once

#include "allocator.hpp"

namespace dze {

// Allocator adapter to allow usage with STL containers.
template <typename T, typename Alloc = allocator>
class typed_allocator : public Alloc
{
public:
    using value_type = T;

    // NOLINTNEXTLINE(modernize-use-equals-default)
    using Alloc::Alloc;

    template <typename U>
    typed_allocator(const typed_allocator<U, Alloc>& other) noexcept
        : Alloc{static_cast<const Alloc&>(other)} {}

    [[nodiscard]] T* allocate(const size_t n)
        noexcept(noexcept(this->allocate_bytes(n * sizeof(T), alignof(T))))
    {
        return static_cast<T*>(this->allocate_bytes(n * sizeof(T), alignof(T)));
    }

    void deallocate(T* const p, const size_t n)
        noexcept(noexcept(this->deallocate_bytes(p, n * sizeof(T), alignof(T))))
    {
        this->deallocate_bytes(p, n * sizeof(T), alignof(T));
    }
};

} // namespace dze
