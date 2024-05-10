project(cells-scripts)
add_library(${PROJECT_NAME} STATIC)

# add sources
target_sources(${PROJECT_NAME} PRIVATE
        simulation_script.cpp
        setup_script.cpp
        setup_script_errors.cpp
        scripts_system.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        simulation_script.h
        setup_script.h
        scripts_pch.h
        setup_script_errors.h
        scripts_system.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        cells-simulation
        cells-render
        cells-ui
)
target_precompile_headers(${PROJECT_NAME} PRIVATE scripts_pch.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
