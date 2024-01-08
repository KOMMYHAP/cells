project(cells-render)

# add SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1 EXACT REQUIRED graphics window)
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-graphics sfml-window)

target_link_libraries(${PROJECT_NAME} PRIVATE cells-simulation)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        world_render.cpp
        cell_render.cpp
        pch.h)


# add headers
target_sources(${PROJECT_NAME} PRIVATE
        world_render.h
        cell_render.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# add pch
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)