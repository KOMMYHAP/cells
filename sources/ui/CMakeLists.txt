project(cells-ui)
add_library(${PROJECT_NAME} STATIC
    ui_registrable_system.cpp
    ui_registrable_system.h
    ui_application_interface.h
)

add_subdirectory(menu_widgets)
add_subdirectory(system)
add_subdirectory(widgets)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
    cells-simulation
    sfml-system
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL2::SDL2
        ImGuiLib
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
