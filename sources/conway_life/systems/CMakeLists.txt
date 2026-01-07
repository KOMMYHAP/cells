add_subdirectory(generated)

target_sources(${PROJECT_NAME}
    PRIVATE
        born_new_life_system.cpp
        collect_neighbours_count_system.cpp
        draw_alive_cell_system.cpp
        process_empty_cell_vitality_system.cpp
        process_alive_cell_vitality_system.cpp
        remove_died_cell_system.cpp
        invalidate_neighbours_system.cpp
        update_game_controller_system.cpp
)