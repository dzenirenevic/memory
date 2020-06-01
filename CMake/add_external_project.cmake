include(FetchContent)

# Simple package manager.
# Does not allow multiple versions of a dependency with the same name.
# Fetches the dependency using FetchContent from a git repository and optionally calls add_subdirectory.
# Arguments to the subdirectories' options can be passed as a list in ARGS.
function(add_external_project proj_name)
    FetchContent_GetProperties(${proj_name})
    string(TOLOWER "${proj_name}" lc_name)
    set(${proj_name}_ADDED NOT ${${lc_name}_POPULATED} PARENT_SCOPE)

    if (${lc_name}_POPULATED)
        cmake_parse_arguments(
            ARG
            ""
            "GIT_TAG"
            ""
            ${ARGN})

        find_package(Git REQUIRED)

        execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-list -n 1 ${ARG_GIT_TAG}
            WORKING_DIRECTORY "${${lc_name}_SOURCE_DIR}"
            OUTPUT_VARIABLE required_rev)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
            WORKING_DIRECTORY "${${lc_name}_SOURCE_DIR}"
            OUTPUT_VARIABLE checked_out_rev)
        string(STRIP ${required_rev} required_rev)
        string(STRIP ${checked_out_rev} checked_out_rev)

        if (NOT ${checked_out_rev} STREQUAL ${required_rev})
            message(
                WARNING
                "External project ${proj_name} added with revision ${required_rev}.\n"
                "Previous revision was ${checked_out_rev}.")
        endif ()
    else ()
        cmake_parse_arguments(
            ARG
            "SKIP_ADD_SUBDIR"
            ""
            "ARGS"
            ${ARGN})

        FetchContent_Declare(${proj_name} ${ARG_UNPARSED_ARGUMENTS})
        FetchContent_Populate(${proj_name})

        if (NOT ${ARG_SKIP_ADD_SUBDIR})
            foreach (arg ${ARG_ARGS})
                STRING(REGEX MATCH "^\(.*\)=\(.*)$" out ${arg})
                set(${CMAKE_MATCH_1} ${CMAKE_MATCH_2})
            endforeach ()

            add_subdirectory(${${proj_name}_SOURCE_DIR} ${${proj_name}_BINARY_DIR} EXCLUDE_FROM_ALL)
        endif ()
    endif ()

    set(${proj_name}_POPULATED True PARENT_SCOPE)
    set(${proj_name}_SOURCE_DIR "${${lc_name}_SOURCE_DIR}" PARENT_SCOPE)
    set(${proj_name}_BINARY_DIR "${${lc_name}_BINARY_DIR}" PARENT_SCOPE)
endfunction()
