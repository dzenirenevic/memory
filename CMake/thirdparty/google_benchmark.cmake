set(proj_name google_benchmark)

include(add_external_project)
include(thirdparty_common)

add_external_project(
    ${proj_name}
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.5.1
    GIT_SHALLOW true
    PREFIX "${thirdparty_prefix}/${proj_name}"
    SOURCE_DIR "${thirdparty_prefix}/${proj_name}/source"
    BINARY_DIR "${thirdparty_binary_dir}/${proj_name}/bin"
    ARGS BENCHMARK_ENABLE_TESTING=OFF)

if (${${proj_name}_ADDED})
    target_compile_options(
        benchmark
        PRIVATE
        -Wno-switch-enum
        -Wno-missing-declarations)

    if (${PROJECT_NAME}_static_analyzer)
        set_target_properties(
            benchmark
            PROPERTIES
            CXX_CLANG_TIDY "${clang_tidy};-checks=-clang-analyzer-deadcode.DeadStores,-modernize-*,-readability-*,-performance-unnecessary-value-param;${clang_tidy_options}")
    endif ()
endif ()

unset(proj_name)
