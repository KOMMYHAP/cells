project(cells-common)
add_library(${PROJECT_NAME} STATIC)

option(ENABLE_PROFILE "Enable profiling" OFF)
add_subdirectory(profile)


# find SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1 EXACT REQUIRED system)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        common_pch.h
        flags.cpp
        command_line.cpp
        time_counter.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        basicdefs.h
        flags.h
        command_line.h
        time_counter.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_precompile_headers(${PROJECT_NAME} PRIVATE common_pch.h)
target_link_libraries(${PROJECT_NAME} PUBLIC
        sfml-system
)