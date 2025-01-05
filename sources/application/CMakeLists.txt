project(cells-application)
add_executable(${PROJECT_NAME} main.cpp)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
        cells-ui
        cells-scripts
)

target_copy_required_dll(${PROJECT_NAME})