project(cells-common)
add_library(${PROJECT_NAME} STATIC)

option(ENABLE_PROFILE "Enable profiling" OFF)

add_subdirectory(profile)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        common_pch.h
        flags.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        basicdefs.h
        flags.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_precompile_headers(${PROJECT_NAME} PRIVATE common_pch.h)