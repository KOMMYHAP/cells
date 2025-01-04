cmake_minimum_required(VERSION 3.26)

project(Cells)

cmake_policy(GET CMP0077 OPTION_POLICY)
if (${OPTION_POLICY} STREQUAL "OLD")
    message(FATAL_ERROR "CMP0077 should be set to correctly set up external libraries")
endif()

add_subdirectory(external)
add_subdirectory(sources)