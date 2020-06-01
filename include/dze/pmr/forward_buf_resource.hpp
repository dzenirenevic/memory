#pragma once

#include <cstddef>
#include <cstdlib>
#include <memory_resource>

#include "../details/forward_allocate.hpp"

namespace dze::pmr {

class forward_buf_resource : public std::pmr::memory_resource
{
public:
    forward_buf_resource(void* const buf, const size_t cap) noexcept
        : m_buf{buf}
        , m_size{cap}
        , m_cap{cap} {}

    forward_buf_resource(const forward_buf_resource&) = delete;

    void release() noexcept
    {
        m_buf = static_cast<std::byte*>(m_buf) - (m_cap - m_size);
        m_size = m_cap;
    }

protected:
    [[nodiscard]] void* do_allocate(const size_t n, const size_t alignment) override
    {
        return details::memory::forward_allocate(m_buf, m_size, n, alignment);
    }

    void do_deallocate(void*, size_t, size_t) override {}

    [[nodiscard]] bool do_is_equal(const memory_resource& rhs) const noexcept override
    {
        return this == &rhs;
    }

private:
    void* m_buf;
    size_t m_size;
    size_t m_cap;
};

} // namespace dze::pmr
