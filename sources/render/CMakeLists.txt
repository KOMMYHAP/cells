project(cells-render)
add_library(${PROJECT_NAME} STATIC)


# add SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1 EXACT REQUIRED graphics window)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        world_render.cpp
        cell_render.cpp
        render_pch.h
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        world_render.h
        cell_render.h
        render_profile_category.h
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