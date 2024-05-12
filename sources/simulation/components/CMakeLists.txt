# add headers
target_sources(${PROJECT_NAME} PUBLIC
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
)

# add sources
target_sources(${PROJECT_NAME} PRIVATE
        cell_position.cpp
        component_storage.cpp
)
