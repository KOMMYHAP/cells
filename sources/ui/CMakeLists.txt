project(cells-ui)
add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PUBLIC
        world_widget.cpp
)
target_sources(${PROJECT_NAME} PRIVATE
        ui_pch.h
        world_widget.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        cells-application-interface
        cells-simulation
        cells-render
        sfml-graphics
)
target_precompile_headers(${PROJECT_NAME} PRIVATE ui_pch.h)

