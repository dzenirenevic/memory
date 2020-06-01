#pragma once

#include <cstdlib>
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

} // namespace dze::details::memory
