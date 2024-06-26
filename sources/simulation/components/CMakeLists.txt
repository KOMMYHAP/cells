target_sources(${PROJECT_NAME} PRIVATE
        cell_brain.h
        cell_id.h
        cell_position.h
        cell_type.h
        cell_age.h
        cell_health.h
        procedure_type.h
        component.h
        component_storage.h
        component_storage.hpp
        component_handle.h
        component_registry.h
        
        cell_position.cpp
        component_storage.cpp
        component_registry.cpp
        
        spawn_place.h
        move_direction.h
)