#pragma once

#include <cstddef>
#include <cstdlib>

#include "resource.hpp"

#include "../details/bidir_allocate.hpp"

namespace dze::pmr {

class bidir_buf_resource : public dze::pmr::memory_resource
{
public:
    bidir_buf_resource(void* const buf, const size_t cap) noexcept
        : m_buf{buf}
        , m_size{cap} {}

    bidir_buf_resource(const bidir_buf_resource&) = delete;

protected:
    [[nodiscard]] void* do_allocate(const size_t n, const size_t alignment) override
    {
        return details::memory::bidir_allocate(m_buf, m_size, n, alignment);
    }

    // Undefined behavior if p is not equal to the return value of the last invocation of
    // do_allocate on this object.
    [[nodiscard]] void* do_reallocate(
        void* const p,
        const size_t n,
        const size_t new_size,
        const size_t alignment) override
    {
        return details::memory::bidir_reallocate(m_buf, m_size, p, n, new_size, alignment);
    }

    // Undefined behavior if p is not equal to the return value of the last invocation of
    // do_allocate on this object.
    void do_deallocate(void* const p, const size_t n, const size_t alignment) override
    {
        details::memory::bidir_deallocate(m_buf, m_size, p, n, alignment);
    }

    [[nodiscard]] bool do_is_equal(const std::pmr::memory_resource& rhs) const noexcept override
    {
        return this == &rhs;
    }

private:
    void* m_buf;
    size_t m_size;
};

} // namespace dze::pmr
