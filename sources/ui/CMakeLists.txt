project(cells-ui)
add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE
        ui_system.cpp
        base_widget.h
)

target_sources(${PROJECT_NAME} PUBLIC
        ui_pch.h
        ui_layout.h
        ui_system.h
)

# add_subdirectory(menu_widgets)
add_subdirectory(widgets)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
        cells-simulation
        sfml-system
        SDL2::SDL2
)
target_precompile_headers(${PROJECT_NAME} PRIVATE ui_pch.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
