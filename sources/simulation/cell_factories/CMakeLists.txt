# add sources
target_sources(${PROJECT_NAME} PRIVATE
        patrol_cell_factory.cpp
        random_cell_factory.cpp
        preset_cell_factory.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        patrol_cell_factory.h
        random_cell_factory.h
        cell_factory_interface.h
        preset_cell_factory.h
)
