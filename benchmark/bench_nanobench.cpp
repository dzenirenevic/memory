#include <forward_list>
#include <memory_resource>

#include <nanobench.h>

#include <dze/allocator.hpp>
#include <dze/bidir_buf_allocator.hpp>
#include <dze/forward_buf_allocator.hpp>
#include <dze/miallocator.hpp>
#include <dze/pmr/bidir_buf_resource.hpp>
#include <dze/pmr/forward_buf_resource.hpp>
#include <dze/pmr/mimalloc_resource.hpp>
#include <dze/typed_allocator.hpp>

int main()
{
    constexpr auto elem_count = 128;

    auto bench = ankerl::nanobench::Bench();
    bench.title("forward workload");

    bench.run(
        "std::allocator",
        [&]
        {
            std::forward_list<int> l;
            for (int i = 0; i != elem_count; ++i)
                l.push_front(i);
        });

    bench.run(
        "dze::allocator",
        [&]
        {
            std::forward_list<int, dze::typed_allocator<int>> l;
            for (int i = 0; i != elem_count; ++i)
                l.push_front(i);
        });

    bench.run(
        "dze::miallocator",
        [&]
        {
            std::forward_list<int, dze::typed_allocator<int, dze::miallocator>> l;
            for (int i = 0; i != elem_count; ++i)
                l.push_front(i);
        });

    {
        constexpr size_t node_size = 16;
        constexpr size_t node_alignment = 8;
        constexpr size_t bidir_alloc_size = node_size + node_alignment;

        dze::allocator galloc;
        auto bytes = galloc.allocate_bytes(elem_count * bidir_alloc_size);

        bench.run(
            "dze::forward_buf_allocator",
            [&]
            {
                using alloc = dze::typed_allocator<int, dze::forward_buf_allocator>;

                std::forward_list<int, alloc> l{{bytes, elem_count * node_size}};
                for (int i = 0; i != elem_count; ++i)
                    l.push_front(i);
            });

        bench.run(
            "dze::bidir_buf_allocator",
            [&]
            {
                using alloc = dze::typed_allocator<int, dze::bidir_buf_allocator>;

                std::forward_list<int, alloc> l{{bytes, elem_count * bidir_alloc_size}};
                for (int i = 0; i != elem_count; ++i)
                    l.push_front(i);
            });

        galloc.deallocate_bytes(bytes, elem_count * bidir_alloc_size);
    }

    bench.run(
        "std::pmr::new_delete_resource",
        [&]
        {
            std::pmr::forward_list<int> l;
            for (int i = 0; i != elem_count; ++i)
                l.push_front(i);
        });

    bench.run(
        "dze::pmr::mimalloc_resource",
        [&]
        {
            std::pmr::forward_list<int> l(dze::pmr::mimalloc_resource());
            for (int i = 0; i != elem_count; ++i)
                l.push_front(i);
        });

    {
        constexpr size_t node_size = 16;
        constexpr size_t node_alignment = 8;
        constexpr size_t bidir_alloc_size = node_size + node_alignment;

        dze::allocator galloc;
        auto bytes = galloc.allocate_bytes(elem_count * bidir_alloc_size);

        bench.run(
            "std::pmr::monotonic_buffer_resource",
            [&]
            {
                std::pmr::monotonic_buffer_resource resource{
                    bytes, elem_count * node_size, std::pmr::null_memory_resource()};
                std::pmr::forward_list<int> l{&resource};
                for (int i = 0; i != elem_count; ++i)
                    l.push_front(i);
            });

        bench.run(
            "dze::pmr::forward_buf_resource",
            [&]
            {
                dze::pmr::forward_buf_resource resource{bytes, elem_count * node_size};
                std::pmr::forward_list<int> l{&resource};
                for (int i = 0; i != elem_count; ++i)
                    l.push_front(i);
            });

        bench.run(
            "dze::pmr::bidir_buf_resource",
            [&]
            {
                dze::pmr::bidir_buf_resource resource{bytes, elem_count * bidir_alloc_size};
                std::pmr::forward_list<int> l{&resource};
                for (int i = 0; i != elem_count; ++i)
                    l.push_front(i);
            });

        galloc.deallocate_bytes(bytes, elem_count * bidir_alloc_size);
    }

    bench.title("stack like workload");

    bench.run(
        "std::allocator",
        [&]
        {
            std::forward_list<int> l;
            for (int i = 0; i != elem_count; ++i)
                l.push_front(i);
            for (int i = 0; i != elem_count; ++i)
                l.pop_front();
            for (int i = 0; i != elem_count; ++i)
                l.push_front(i);
        });

    bench.run(
        "dze::allocator",
        [&]
        {
            std::forward_list<int, dze::typed_allocator<int>> l;
            for (int i = 0; i != elem_count; ++i)
                l.push_front(i);
            for (int i = 0; i != elem_count; ++i)
                l.pop_front();
            for (int i = 0; i != elem_count; ++i)
                l.push_front(i);
        });

    bench.run(
        "dze::miallocator",
        [&]
        {
            std::forward_list<int, dze::typed_allocator<int, dze::miallocator>> l;
            for (int i = 0; i != elem_count; ++i)
                l.push_front(i);
            for (int i = 0; i != elem_count; ++i)
                l.pop_front();
            for (int i = 0; i != elem_count; ++i)
                l.push_front(i);
        });

    {
        constexpr size_t node_size = 16;
        constexpr size_t node_alignment = 8;
        constexpr size_t bidir_alloc_size = node_size + node_alignment;

        dze::allocator galloc;
        auto bytes = galloc.allocate_bytes(elem_count * std::max(2 * node_size, bidir_alloc_size));

        bench.run(
            "dze::forward_buf_allocator",
            [&]
            {
                using alloc = dze::typed_allocator<int, dze::forward_buf_allocator>;

                std::forward_list<int, alloc> l{{bytes, elem_count * 2 * node_size}};
                for (int i = 0; i != elem_count; ++i)
                    l.push_front(i);
                for (int i = 0; i != elem_count; ++i)
                    l.pop_front();
                for (int i = 0; i != elem_count; ++i)
                    l.push_front(i);
            });

        bench.run(
            "dze::bidir_buf_allocator",
            [&]
            {
                using alloc = dze::typed_allocator<int, dze::bidir_buf_allocator>;

                std::forward_list<int, alloc> l{{bytes, elem_count * bidir_alloc_size}};
                for (int i = 0; i != elem_count; ++i)
                    l.push_front(i);
                for (int i = 0; i != elem_count; ++i)
                    l.pop_front();
                for (int i = 0; i != elem_count; ++i)
                    l.push_front(i);
            });

        galloc.deallocate_bytes(bytes, elem_count * std::max(2 * node_size, bidir_alloc_size));
    }

    bench.run(
        "std::pmr::new_delete_resource",
        [&]
        {
            std::pmr::forward_list<int> l;
            for (int i = 0; i != elem_count; ++i)
                l.push_front(i);
            for (int i = 0; i != elem_count; ++i)
                l.pop_front();
            for (int i = 0; i != elem_count; ++i)
                l.push_front(i);
        });

    bench.run(
        "dze::pmr::mimalloc_resource",
        [&]
        {
            std::pmr::forward_list<int> l(dze::pmr::mimalloc_resource());
            for (int i = 0; i != elem_count; ++i)
                l.push_front(i);
            for (int i = 0; i != elem_count; ++i)
                l.pop_front();
            for (int i = 0; i != elem_count; ++i)
                l.push_front(i);
        });

    {
        constexpr size_t node_size = 16;
        constexpr size_t node_alignment = 8;
        constexpr size_t bidir_alloc_size = node_size + node_alignment;

        dze::allocator galloc;
        auto bytes = galloc.allocate_bytes(elem_count * std::max(2 * node_size, bidir_alloc_size));

        bench.run(
            "std::pmr::monotonic_buffer_resource",
            [&]
            {
                std::pmr::monotonic_buffer_resource resource{
                    bytes, 2 * elem_count * node_size, std::pmr::null_memory_resource()};
                std::pmr::forward_list<int> l{&resource};
                for (int i = 0; i != elem_count; ++i)
                    l.push_front(i);
                for (int i = 0; i != elem_count; ++i)
                    l.pop_front();
                for (int i = 0; i != elem_count; ++i)
                    l.push_front(i);
            });

        bench.run(
            "dze::pmr::forward_buf_resource",
            [&]
            {
                dze::pmr::forward_buf_resource resource{bytes, 2 * elem_count * node_size};
                std::pmr::forward_list<int> l{&resource};
                for (int i = 0; i != elem_count; ++i)
                    l.push_front(i);
                for (int i = 0; i != elem_count; ++i)
                    l.pop_front();
                for (int i = 0; i != elem_count; ++i)
                    l.push_front(i);
            });

        bench.run(
            "dze::pmr::bidir_buf_resource",
            [&]
            {
                dze::pmr::bidir_buf_resource resource{bytes, elem_count * bidir_alloc_size};
                std::pmr::forward_list<int> l{&resource};
                for (int i = 0; i != elem_count; ++i)
                    l.push_front(i);
                for (int i = 0; i != elem_count; ++i)
                    l.pop_front();
                for (int i = 0; i != elem_count; ++i)
                    l.push_front(i);
            });

        galloc.deallocate_bytes(bytes, elem_count * std::max(2 * node_size, bidir_alloc_size));
    }
}
