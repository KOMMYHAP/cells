add_subdirectory(generated)

target_sources(${PROJECT_NAME} PRIVATE
        world_rasterization_system.cpp
        world_rasterization_lock_system.cpp
        world_rasterization_unlock_system.cpp
)