target_sources(${PROJECT_NAME} PRIVATE
        age_system.cpp
        brain_simulation_system.h
        brain_simulation_system.cpp
        energy_leak_system.cpp
        energy_decrease_system.cpp
        graveyard_system.cpp
        graveyard_system.h
        alive_cells_statistics_system.h
        alive_cells_statistics_system.cpp
        spawn_system.cpp
        spawn_system.h
        spawn_places_statistics_system.h
        spawn_places_statistics_system.cpp
        keep_population_system.h
        keep_population_system.cpp
        death_from_age_statistics_system.h
        death_from_age_statistics_system.cpp
        death_from_empty_energy_statistics_system.h
        death_from_empty_energy_statistics_system.cpp
)

add_subdirectory(generated)