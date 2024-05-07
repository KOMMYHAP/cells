project(cells-render)
add_library(${PROJECT_NAME} STATIC)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        world_render.cpp
        render_pch.h
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        world_render.h
        render_profile_category.h
        world_render_parameters.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
        cells-common
        cells-simulation
        sfml-graphics
        sfml-window
)


# add pch
target_precompile_headers(${PROJECT_NAME} PRIVATE render_pch.h)