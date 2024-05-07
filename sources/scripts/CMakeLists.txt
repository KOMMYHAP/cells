project(cells-scripts)
add_library(${PROJECT_NAME} STATIC)

# add sources
target_sources(${PROJECT_NAME} PRIVATE
        tick_script.cpp
        setup_script.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        tick_script.h
        setup_script.h
        scripts_pch.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        cells-simulation
        cells-render
        cells-ui
)
target_precompile_headers(${PROJECT_NAME} PRIVATE scripts_pch.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
