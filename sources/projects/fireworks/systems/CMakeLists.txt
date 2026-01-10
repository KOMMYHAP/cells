add_subdirectory(generated)

target_sources(${PROJECT_NAME}
    PRIVATE
        update_emitter_system.cpp
        update_particle_lifetime_system.cpp
        update_particle_color_system.cpp
        update_particle_gravity_system.cpp
        update_particle_position_system.cpp
        update_particle_velocity_system.cpp
        draw_empty_cell_system.cpp
        draw_particle_system.cpp
        emit_new_particle_system.cpp
)