#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>
#include <memory_resource>

#include <mimalloc.h>

namespace dze::pmr {

namespace details::memory {

class mimalloc_resource_t : public std::pmr::memory_resource
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

    void do_deallocate(void* p, size_t, size_t) override
    {
        mi_free(p);
    }

    [[nodiscard]] bool do_is_equal(const memory_resource&) const noexcept override
    {
        return true;
    }
};

mimalloc_resource_t mimalloc_resource;

} // namespace detail::memory

[[nodiscard]] inline std::pmr::memory_resource* mimalloc_resource() noexcept
{
    return &details::memory::mimalloc_resource;
}

} // namespace dze::pmr
