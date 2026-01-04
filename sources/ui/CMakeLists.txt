project(cells-ui)
add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE
        ui_registrable_system.cpp
)

target_sources(${PROJECT_NAME} PUBLIC
        ui_registrable_system.h
        ui_application_interface.h
        ui_config.h
)

add_subdirectory(system)
add_subdirectory(menu_widgets)
add_subdirectory(widgets)
add_subdirectory(systems_ecs)

target_link_libraries(${PROJECT_NAME} PUBLIC
        cells-simulation
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        SDL3::SDL3
        ImGuiLib
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
