#pragma once

#include <cstdlib>
#include <cstring>
#include <memory>
#include <new>

namespace dze::details::memory {

[[nodiscard]] inline void* forward_allocate(
    void*& buf, size_t& size, const size_t n, const size_t alignment)
{
    if (std::align(alignment, n, buf, size) == nullptr)
        throw std::bad_alloc{};

    const auto t = buf;
    buf = static_cast<std::byte*>(buf) + n;
    size -= n;
    return t;
}

[[nodiscard]] inline void* forward_reallocate(
    void*& buf,
    size_t& size,
    void* const p,
    const size_t n,
    const size_t new_size,
    const size_t alignment)
{
    if (new_size > n)
    {
        void* buf_nx = static_cast<std::byte*>(buf) - n;
        if (p == buf_nx)
        {
            if (size < new_size - n)
                throw std::bad_alloc{};

            size -= new_size - n;
            return p;
        }
        else
        {
            auto size_nx = size + n;
            const auto bytes = forward_allocate(buf_nx, size_nx, n, alignment);
            std::memcpy(bytes, p, n);
            buf = buf_nx;
            size = size_nx;
            return bytes;
        }
    }
    else
    {
        if (p == static_cast<std::byte*>(buf) - n)
            size += n - new_size;

        return p;
    }
}

} // namespace dze::details::memory
