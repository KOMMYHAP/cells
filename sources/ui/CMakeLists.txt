project(cells-ui)
add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE
        ui_system.cpp
        status_panel.cpp
        world_widget.cpp
)

target_sources(${PROJECT_NAME} PUBLIC
        ui_pch.h
        ui_layout.h
        ui_system.h
        status_panel.h
        ui_widget.h
        world_widget.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        cells-simulation
        sfml-graphics
)
target_precompile_headers(${PROJECT_NAME} PRIVATE ui_pch.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
