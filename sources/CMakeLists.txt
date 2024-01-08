# Simulation
add_library(cells-simulation STATIC)
target_include_directories(cells-simulation PRIVATE common)
add_subdirectory(simulation)


# Render
add_library(cells-render STATIC)
target_include_directories(cells-render PRIVATE common)
add_subdirectory(render)


# Main
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE cells-simulation cells-render)
