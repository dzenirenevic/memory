#pragma once

#include <cstdlib>
#include <memory_resource>

namespace dze::pmr {

class memory_resource : public std::pmr::memory_resource
{
public:
    [[nodiscard]] void* reallocate(
        void* const p,
        const size_t n,
        const size_t new_size,
        const size_t alignment)
    {
        return do_reallocate(p, n, new_size, alignment);
    }

protected:
    [[nodiscard]] virtual void* do_reallocate(
        void* const p,
        const size_t n,
        const size_t new_size,
        const size_t alignment)
    {
        void* const bytes = allocate(new_size, alignment);
        std::memcpy(bytes, p, n);
        deallocate(p, n, alignment);
        return bytes;
    }
};

} // namespace dze::pmr
