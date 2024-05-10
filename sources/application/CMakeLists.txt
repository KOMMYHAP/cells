project(cells-application)
add_executable(${PROJECT_NAME} main.cpp)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        main.cpp
        main_loop.cpp
        application_pch.h
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        main_loop.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
        cells-common
        cells-processor
        cells-render
        cells-simulation
        sfml-system
        cells-ui
        cells-scripts
)


# add pch
target_precompile_headers(${PROJECT_NAME} PRIVATE application_pch.h)

target_copy_required_dll(${PROJECT_NAME})