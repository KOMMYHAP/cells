add_executable(${PROJECT_NAME})


# add SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1 EXACT REQUIRED graphics window)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window)


# add Quadtree
target_link_libraries(${PROJECT_NAME} PRIVATE QuadTree)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        main.cpp pch.h
        field.h field.cpp
        cell.h cell.cpp
        cell_search_proxy.h cell_search_proxy.cpp)
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)