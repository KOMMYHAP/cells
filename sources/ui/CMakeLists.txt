project(cells-ui)
add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE
        ui_system.cpp
        custom_render_widget.h
        base_widget.h
        root_widget.h
        root_widget.hpp
        root_widget.cpp
)

target_sources(${PROJECT_NAME} PUBLIC
        ui_pch.h
        ui_layout.h
        ui_system.h
)

add_subdirectory(widgets)

target_link_libraries(${PROJECT_NAME} PRIVATE
        cells-simulation
        sfml-graphics
        ImGui-SFML
)
target_precompile_headers(${PROJECT_NAME} PRIVATE ui_pch.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
