project(cells-application)
add_executable(${PROJECT_NAME} main.cpp)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC .)

target_link_libraries(${PROJECT_NAME} PUBLIC
        cells-ui
        SDL3::SDL3
)

target_copy_required_dll(${PROJECT_NAME})