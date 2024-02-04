project(cells-simulation)
add_library(${PROJECT_NAME} STATIC)


# find SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1 EXACT REQUIRED system)


# add sub sources
add_subdirectory(components)
add_subdirectory(systems)
add_subdirectory(procedures)
add_subdirectory(genetic)
add_subdirectory(cell_factories)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        simulation.cpp
        world_interface.h
        simulation_pch.h
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        simulation.h
        simulation_profile_category.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
        cells-common
        cells-processor
        sfml-system
)


# add pch
target_precompile_headers(${PROJECT_NAME} PRIVATE simulation_pch.h)