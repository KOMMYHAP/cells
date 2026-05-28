project(creatures)
add_executable(${PROJECT_NAME} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE
        main.cpp
        pch.h
        ecs_config.h
        world_simulation/creature_simulation/brain_simulation/brain.hpp
        world_simulation/creature_simulation/brain_simulation/brain.h
)

target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        cells-common
        cells-ui
        EnTT::EnTT
        SDL3::SDL3
        ImGuiLib
)
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)
target_copy_required_dll(${PROJECT_NAME})