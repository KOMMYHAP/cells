project(fireworks)
add_executable(${PROJECT_NAME} main.cpp)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        main.cpp
        pch.h
        game_controller.cpp
        game_controller.h
        game_config.h
        particle_random.cpp
        particle_random.h
)

add_subdirectory(components/generated)
add_subdirectory(systems)

target_include_directories(${PROJECT_NAME} PUBLIC .)

target_link_libraries(${PROJECT_NAME} PUBLIC
        cells-ui
)
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)

target_copy_required_dll(${PROJECT_NAME})