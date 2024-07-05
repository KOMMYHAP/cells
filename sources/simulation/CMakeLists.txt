project(cells-simulation)
add_library(${PROJECT_NAME} STATIC)


# add sub sources
add_subdirectory(components)
#add_subdirectory(systems)
add_subdirectory(procedures)
add_subdirectory(genetic)
add_subdirectory(cell_factories)
add_subdirectory(systems_ecs)
add_subdirectory(simulation)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        simulation_pch.h
        simulation_registrable_system.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        simulation_registrable_system.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
        cells-common
        cells-processor
        sfml-system
        EnTT::EnTT
)


# add pch
target_precompile_headers(${PROJECT_NAME} PRIVATE simulation_pch.h)