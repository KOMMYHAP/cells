target_sources(${PROJECT_NAME} PRIVATE
        graveyarded_cell_exclude.h
        age_system.cpp
        brain_simulation_system.cpp
        energy_leak_system.cpp
        energy_decrease_system.cpp
        graveyard_system.cpp
        alive_cells_statistics_system.cpp
        spawn_system.cpp
        spawn_places_statistics_system.cpp
        keep_population_system.cpp
        death_from_age_statistics_system.cpp
        death_from_empty_energy_statistics_system.cpp
)

add_subdirectory(generated)