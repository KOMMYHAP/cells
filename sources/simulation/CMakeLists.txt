project(cells-simulation)
add_library(${PROJECT_NAME} STATIC)


# add sub sources
add_subdirectory(components)
add_subdirectory(systems)
add_subdirectory(procedures)
add_subdirectory(genetic)
add_subdirectory(cell_factories)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        simulation.cpp
        world.cpp
        simulation_pch.h
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        simulation.h
        base_script.h
        world.h
        world_parameters.h
        simulation_profile_category.h
        simulation_parameters.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
        cells-common
        cells-processor
        cells-application-interface
        sfml-system
)


# add pch
target_precompile_headers(${PROJECT_NAME} PRIVATE simulation_pch.h)