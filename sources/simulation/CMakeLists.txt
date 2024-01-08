project(cells-simulation)

# find SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1 EXACT REQUIRED system)
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-system)


# add Quadtree
target_link_libraries(${PROJECT_NAME} PRIVATE quadtree)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        field.cpp
        cell.cpp
        cell_search_proxy.cpp
        brain.cpp
        simulation.cpp
        pch.h)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        field.h
        cell.h
        cell_search_proxy.h
        brain.h
        simulation.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# add pch
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)