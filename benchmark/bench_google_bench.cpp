#include <forward_list>
#include <memory_resource>

#include <benchmark/benchmark.h>

#include <dze/allocator.hpp>
#include <dze/bidir_buf_allocator.hpp>
#include <dze/forward_buf_allocator.hpp>
#include <dze/miallocator.hpp>
#include <dze/pmr/bidir_buf_resource.hpp>
#include <dze/pmr/forward_buf_resource.hpp>
#include <dze/pmr/mimalloc_resource.hpp>
#include <dze/typed_allocator.hpp>

constexpr auto elem_count = 128;
constexpr size_t node_size = 16;
constexpr size_t node_alignment = 8;
constexpr size_t bidir_alloc_size = node_size + node_alignment;

dze::allocator galloc;

namespace {

void fwd_workload_std_allocator(benchmark::State& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        std::forward_list<int> l;
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }
}

void fwd_workload_dze_allocator(benchmark::State& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        std::forward_list<int, dze::typed_allocator<int>> l;
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }
}

void fwd_workload_dze_miallocator(benchmark::State& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        std::forward_list<int, dze::typed_allocator<int, dze::miallocator>> l;
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }
}

void fwd_workload_dze_forward_buf_allocator(benchmark::State& state)
{
    auto bytes = galloc.allocate_bytes(elem_count * bidir_alloc_size);

    using alloc = dze::typed_allocator<int, dze::forward_buf_allocator>;

    for ([[maybe_unused]] auto _ : state)
    {
        std::forward_list<int, alloc> l{{bytes, elem_count * node_size}};
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }

    galloc.deallocate_bytes(bytes, elem_count * bidir_alloc_size);
}

void fwd_workload_dze_bidir_buf_allocator(benchmark::State& state)
{
    auto bytes = galloc.allocate_bytes(elem_count * bidir_alloc_size);

    using alloc = dze::typed_allocator<int, dze::bidir_buf_allocator>;

    for ([[maybe_unused]] auto _ : state)
    {
        std::forward_list<int, alloc> l{{bytes, elem_count * bidir_alloc_size}};
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }

    galloc.deallocate_bytes(bytes, elem_count * bidir_alloc_size);
}

void fwd_workload_std_pmr_new_delete_resource(benchmark::State& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        std::pmr::forward_list<int> l;
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }
}

void fwd_workload_dze_pmr_mimalloc_resource(benchmark::State& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        std::pmr::forward_list<int> l(dze::pmr::mimalloc_resource());
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }
}

void fwd_workload_std_pmr_monotonic_buffer_resource(benchmark::State& state)
{
    auto bytes = galloc.allocate_bytes(elem_count * node_size);

    for ([[maybe_unused]] auto _ : state)
    {
        std::pmr::monotonic_buffer_resource resource{
            bytes, elem_count * node_size, std::pmr::null_memory_resource()};
        std::pmr::forward_list<int> l{&resource};
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }

    galloc.deallocate_bytes(bytes, elem_count * node_size);
}

void fwd_workload_dze_pmr_forward_buf_resource(benchmark::State& state)
{
    auto bytes = galloc.allocate_bytes(elem_count * node_size);

    for ([[maybe_unused]] auto _ : state)
    {
        dze::pmr::forward_buf_resource resource{bytes, elem_count * node_size};
        std::pmr::forward_list<int> l{&resource};
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }

    galloc.deallocate_bytes(bytes, elem_count * node_size);
}

void fwd_workload_dze_pmr_bidir_buf_resource(benchmark::State& state)
{
    auto bytes = galloc.allocate_bytes(elem_count * bidir_alloc_size);

    for ([[maybe_unused]] auto _ : state)
    {
        dze::pmr::bidir_buf_resource resource{bytes, elem_count * bidir_alloc_size};
        std::pmr::forward_list<int> l{&resource};
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }

    galloc.deallocate_bytes(bytes, elem_count * bidir_alloc_size);
}

void stack_workload_std_allocator(benchmark::State& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        std::forward_list<int> l;
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
        for (int i = 0; i != elem_count; ++i)
            l.pop_front();
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }
}

void stack_workload_dze_allocator(benchmark::State& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        std::forward_list<int, dze::typed_allocator<int>> l;
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
        for (int i = 0; i != elem_count; ++i)
            l.pop_front();
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }
}

void stack_workload_dze_miallocator(benchmark::State& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        std::forward_list<int, dze::typed_allocator<int, dze::miallocator>> l;
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
        for (int i = 0; i != elem_count; ++i)
            l.pop_front();
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }
}

void stack_workload_dze_forward_buf_allocator(benchmark::State& state)
{
    auto bytes = galloc.allocate_bytes(2 * elem_count * node_size);

    using alloc = dze::typed_allocator<int, dze::forward_buf_allocator>;

    for ([[maybe_unused]] auto _ : state)
    {
        std::forward_list<int, alloc> l{{bytes, 2 * elem_count * node_size}};
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
        for (int i = 0; i != elem_count; ++i)
            l.pop_front();
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }

    galloc.deallocate_bytes(bytes, 2 * elem_count * node_size);
}

void stack_workload_dze_bidir_buf_allocator(benchmark::State& state)
{
    auto bytes = galloc.allocate_bytes(elem_count * bidir_alloc_size);

    using alloc = dze::typed_allocator<int, dze::bidir_buf_allocator>;

    for ([[maybe_unused]] auto _ : state)
    {
        std::forward_list<int, alloc> l{{bytes, elem_count * bidir_alloc_size}};
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
        for (int i = 0; i != elem_count; ++i)
            l.pop_front();
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }

    galloc.deallocate_bytes(bytes, elem_count * bidir_alloc_size);
}

void stack_workload_std_pmr_new_delete_resource(benchmark::State& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        std::pmr::forward_list<int> l;
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
        for (int i = 0; i != elem_count; ++i)
            l.pop_front();
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }
}

void stack_workload_dze_pmr_mimalloc_resource(benchmark::State& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        std::pmr::forward_list<int> l(dze::pmr::mimalloc_resource());
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
        for (int i = 0; i != elem_count; ++i)
            l.pop_front();
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }
}

void stack_workload_std_pmr_monotonic_buffer_resource(benchmark::State& state)
{
    auto bytes = galloc.allocate_bytes(2 * elem_count * node_size);

    for ([[maybe_unused]] auto _ : state)
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
    }

    galloc.deallocate_bytes(bytes, 2 * elem_count * node_size);
}

void stack_workload_dze_pmr_forward_buf_resource(benchmark::State& state)
{
    auto bytes = galloc.allocate_bytes(2 * elem_count * node_size);

    for ([[maybe_unused]] auto _ : state)
    {
        dze::pmr::forward_buf_resource resource{bytes, 2 * elem_count * node_size};
        std::pmr::forward_list<int> l{&resource};
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
        for (int i = 0; i != elem_count; ++i)
            l.pop_front();
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }

    galloc.deallocate_bytes(bytes, elem_count * 2 * node_size);
}

void stack_workload_dze_pmr_bidir_buf_resource(benchmark::State& state)
{
    auto bytes = galloc.allocate_bytes(elem_count * bidir_alloc_size);

    for ([[maybe_unused]] auto _ : state)
    {
        dze::pmr::bidir_buf_resource resource{bytes, elem_count * bidir_alloc_size};
        std::pmr::forward_list<int> l{&resource};
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
        for (int i = 0; i != elem_count; ++i)
            l.pop_front();
        for (int i = 0; i != elem_count; ++i)
            l.push_front(i);
    }

    galloc.deallocate_bytes(bytes, elem_count * bidir_alloc_size);
}

} // namespace

BENCHMARK(fwd_workload_std_allocator);
BENCHMARK(fwd_workload_dze_allocator);
BENCHMARK(fwd_workload_dze_miallocator);
BENCHMARK(fwd_workload_dze_forward_buf_allocator);
BENCHMARK(fwd_workload_dze_bidir_buf_allocator);
BENCHMARK(fwd_workload_std_pmr_new_delete_resource);
BENCHMARK(fwd_workload_dze_pmr_mimalloc_resource);
BENCHMARK(fwd_workload_std_pmr_monotonic_buffer_resource);
BENCHMARK(fwd_workload_dze_pmr_forward_buf_resource);
BENCHMARK(fwd_workload_dze_pmr_bidir_buf_resource);

BENCHMARK(stack_workload_std_allocator);
BENCHMARK(stack_workload_dze_allocator);
BENCHMARK(stack_workload_dze_miallocator);
BENCHMARK(stack_workload_dze_forward_buf_allocator);
BENCHMARK(stack_workload_dze_bidir_buf_allocator);
BENCHMARK(stack_workload_std_pmr_new_delete_resource);
BENCHMARK(stack_workload_dze_pmr_mimalloc_resource);
BENCHMARK(stack_workload_std_pmr_monotonic_buffer_resource);
BENCHMARK(stack_workload_dze_pmr_forward_buf_resource);
BENCHMARK(stack_workload_dze_pmr_bidir_buf_resource);

BENCHMARK_MAIN();
