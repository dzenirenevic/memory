#pragma once

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <new>

namespace dze::details::memory {

static_assert(
    __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__,
    "This allocator is only available in little endian systems");

[[nodiscard]] inline void* bidir_allocate(
    void*& buf, size_t& size, const size_t n, const size_t alignment)
{
    void* buf_nx = static_cast<std::byte*>(buf) + 1;
    auto size_nx = size;
    if (std::align(alignment, n, buf_nx, size_nx) == nullptr)
        throw std::bad_alloc{};

    if (size_nx == 0)
        throw std::bad_alloc{};

    const auto offset = size - size_nx + 1;
    assert(offset <= std::numeric_limits<uint64_t>::max() / 2);

    // Store offset in one byte if offset is less than 8.
    if (offset < 8)
    {
        constexpr auto mask = uint8_t{1} << 7;
        const auto offset_8 = static_cast<uint8_t>(offset) | mask;
        std::memcpy(static_cast<std::byte*>(buf_nx) - sizeof(uint8_t), &offset_8, sizeof(uint8_t));
    }
    else
    {
        const auto offset_64 = static_cast<uint64_t>(offset);
        std::memcpy(
            static_cast<std::byte*>(buf_nx) - sizeof(uint64_t), &offset_64, sizeof(uint64_t));
    }

    buf = static_cast<std::byte*>(buf_nx) + n;
    size = size_nx - n;
    return buf_nx;
}

[[nodiscard]] inline void* bidir_reallocate(
    [[maybe_unused]] void*& buf,
    size_t& size,
    void* const p,
    const size_t n,
    const size_t new_size,
    size_t)
{
    assert(p == static_cast<std::byte*>(buf) - n);

    if (new_size > n)
    {
        if (size < new_size - n)
            throw std::bad_alloc{};

        size -= new_size - n;
    }
    else
        size += n - new_size;

    return p;
}

inline void bidir_deallocate(
    void*& buf, size_t& size, void* const p, const size_t n, size_t) noexcept
{
    assert(p == static_cast<std::byte*>(buf) - n);

    uint8_t first_octet;
    std::memcpy(&first_octet, static_cast<std::byte*>(p) - sizeof(uint8_t), sizeof(uint8_t));

    constexpr auto mask = uint8_t{1} << 7;
    size_t offset;
    if ((first_octet & mask) != 0)
        offset = first_octet & ~mask;
    else
    {
        uint64_t offset_64;
        std::memcpy(&offset_64, static_cast<std::byte*>(p) - sizeof(uint64_t), sizeof(uint64_t));
        offset = offset_64;
    }

    buf = static_cast<std::byte*>(p) - offset;
    size += n + offset;
}

} // namespace dze::details::memory
