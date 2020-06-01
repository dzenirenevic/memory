# Memory management utilities for C++

This library introduces a simplified extension to the C++17 allocator API inline with polymorphic memory resources.

Allocators listed below are not templated on a type and provide `allocate_bytes(size, align)` and `deallocate_bytes(ptr, size, align)` methods.

 - `dze::allocator` is a stateless allocator based on global new allocator. Its `allocate_bytes` method is `noexcept`.
 - `dze::forward_buf_allocator` is based on `std::pmr::monotonic_memory_resource` without the virtual calls and an upstream memory resource and is intented to provide highest throughput when provided with a buffer of adequate size.
 - `dze::bidir_buf_allocator` works best with a stack like workflow without having to release the whole buffer.

To use the above with STL containers, `dze::typed_allocator` is provided. It is an allocator adapter that redirects calls to `allocate_bytes` and `deallocate_bytes` accordingly.

Furthermore, extensions for polymorphic memory resources are also provided.

 - `dze::pmr::polymorphic_allocator` is a wrapper around `std::polymorphic_allocator<std::byte>` that provides the `allocate_bytes` and `deallocate_bytes` methods in C++17.
 - `dze::pmr::forward_buf_resource` is the polymorphic version of `dze::forward_buf_allocator`.
 - `dze::pmr::bidir_buf_resource` is the polymorphic version of `dze::bidir_buf_allocator`.
