add_subdirectory(generated)

target_sources(${PROJECT_NAME}
    PRIVATE
        particles_draw_system.cpp
        particles_init_system.cpp
        particles_update_color_system.cpp
        particles_update_gravity_system.cpp
        particles_update_lifetime_system.cpp
        particles_update_position_system.cpp
        particles_update_stats_system.cpp
        emitters_init_system.cpp
        emitters_update_lifetime_system.cpp
        emitters_update_stats_system.cpp
        empty_cell_draw_system.cpp
)