set(proj_name mimalloc)

include(add_external_project)
include(thirdparty_common)

add_external_project(
    ${proj_name}
    GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
    GIT_TAG v1.6.3
    GIT_SHALLOW true
    PREFIX "${thirdparty_prefix}/${proj_name}"
    SOURCE_DIR "${thirdparty_prefix}/${proj_name}/source"
    BINARY_DIR "${thirdparty_binary_dir}/${proj_name}/bin")

target_compile_options(
    mimalloc
    PUBLIC -Wno-pedantic
    PRIVATE -Wno-missing-declarations)
