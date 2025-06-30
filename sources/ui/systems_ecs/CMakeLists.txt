add_subdirectory(generated)

target_sources(${PROJECT_NAME} PRIVATE
        world_rasterization_system.cpp
)