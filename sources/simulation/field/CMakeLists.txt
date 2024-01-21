add_subdirectory(quadtree)
add_subdirectory(quadtree_my)

# add sources
target_sources(${PROJECT_NAME} PRIVATE
        field.cpp
        field_grid.cpp
        field_iterator.cpp)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        field.h
        field_grid.h
        field_iterator.h
        field_iterator.hpp)
