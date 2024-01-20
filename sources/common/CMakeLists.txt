project(cells-common)
add_library(${PROJECT_NAME} STATIC)

option(ENABLE_PROFILE "Enable profiling" OFF)
add_subdirectory(profile)



# add sources
target_sources(${PROJECT_NAME} PRIVATE
        common_pch.h
        flags.cpp
        command_line.cpp
        sample_counter.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        basicdefs.h
        flags.h
        command_line.h
        sample_counter.h
        sample_counter.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_precompile_headers(${PROJECT_NAME} PRIVATE common_pch.h)