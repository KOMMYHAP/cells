project(cells-simulation)
add_library(${PROJECT_NAME} STATIC)


# find SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1 EXACT REQUIRED system)


# add sub sources
add_subdirectory(brain)
add_subdirectory(field)
add_subdirectory(processor)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        simulation.cpp
        simulation_pch.h
)
target_link_libraries(${PROJECT_NAME} PRIVATE quadtree common)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        simulation.h)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
        common
        sfml-system
)


# add pch
target_precompile_headers(${PROJECT_NAME} PRIVATE simulation_pch.h)