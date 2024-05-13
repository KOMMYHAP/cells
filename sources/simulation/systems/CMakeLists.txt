# add sources
target_sources(${PROJECT_NAME} PRIVATE
        position_system.cpp
        brain_system.cpp
        type_system.cpp
        health_system.cpp
        id_system.cpp
        simulation_virtual_machine.cpp
        spawn_system.cpp
        graveyard_system.cpp
        selection_system.cpp
        age_system.cpp
        spawner.cpp
        destroyer.cpp
        system.cpp
        system_registry.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        position_system.h
        position_system.hpp
        id_system.h
        id_system.hpp
        brain_system.h
        type_system.h
        health_system.h
        simulation_virtual_machine.h
        simulation_virtual_machine.hpp
        spawn_system.h
        graveyard_system.h
        selection_system.h
        age_system.h
        spawner.h
        destroyer.h
        spawn_properties.h
        system.h
        system_handle.h
        system_registry.h
)
