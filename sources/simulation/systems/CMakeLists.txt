# add sources
target_sources(${PROJECT_NAME} PRIVATE
        brain_system.cpp
        type_system.cpp
        health_system.cpp
        id_system.cpp
        spawn_system.cpp
        graveyard_system.cpp
        selection_system.cpp
        age_system.cpp
        spawner.cpp
        destroyer.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        id_system.h
        id_system.hpp
        brain_system.h
        type_system.h
        health_system.h
        spawn_system.h
        graveyard_system.h
        selection_system.h
        age_system.h
        spawner.h
        destroyer.h
        spawn_properties.h
        system_typed.h
)
