project(conway-life)
add_executable(${PROJECT_NAME} main.cpp)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        main.cpp
        pch.h
        conway_game.cpp
        conway_game.h
        field.cpp
        field.h
        field_utilities.cpp
        field_utilities.h
        conway_debug_widget.h
        conway_debug_widget.cpp
        game_config.h
        sdl_utils.cpp
        sdl_utils.h
)

add_subdirectory(components/generated)
add_subdirectory(systems)

target_include_directories(${PROJECT_NAME} PUBLIC .)

target_link_libraries(${PROJECT_NAME} PUBLIC
        cells-ui
)
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)

target_copy_required_dll(${PROJECT_NAME})