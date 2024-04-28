project(cells-universe)
add_executable(${PROJECT_NAME} main.cpp)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        world_white.cpp
        main.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        world_white.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
        cells-common
        cells-processor
        cells-render
        cells-simulation
        sfml-system
)


# add pch
target_precompile_headers(${PROJECT_NAME} PRIVATE universe_pch.h)

target_copy_required_dll(${PROJECT_NAME})