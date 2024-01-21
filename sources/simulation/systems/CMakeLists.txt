# add sources
target_sources(${PROJECT_NAME} PRIVATE
        position_system.cpp
        position_grid.cpp
        brain_system.cpp
        type_system.cpp
        id_system.cpp
        simulation_virtual_machine.cpp
        cell_factory.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        position_system.h
        position_grid.h
        id_system.h
        brain_system.h
        type_system.h
        simulation_virtual_machine.h
        cell_factory.h
)
