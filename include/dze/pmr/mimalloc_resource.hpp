#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>

#include <mimalloc.h>

#include "resource.hpp"

namespace dze::pmr {

namespace details::memory {

class mimalloc_resource_t : public dze::pmr::memory_resource
{
protected:
    [[nodiscard]] void* do_allocate(const size_t n, const size_t alignment) override
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

    [[nodiscard]] void* do_reallocate(
        void* const p,
        size_t,
        const size_t new_size,
        const size_t alignment = alignof(std::max_align_t)) override
    {
        void* bytes;
        if (alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
            bytes = mi_realloc_aligned(p, new_size, alignment);
        else
            bytes = mi_realloc(p, new_size);

        if (bytes == nullptr)
            throw std::bad_alloc{};

        return bytes;
    }

    void do_deallocate(void* p, size_t, size_t) override
    {
        mi_free(p);
    }

    [[nodiscard]] bool do_is_equal(const std::pmr::memory_resource&) const noexcept override
    {
        return true;
    }
};

} // namespace detail::memory

[[nodiscard]] inline std::pmr::memory_resource* mimalloc_resource() noexcept
{
    static details::memory::mimalloc_resource_t mimalloc_resource;
    return &mimalloc_resource;
}

} // namespace dze::pmr
