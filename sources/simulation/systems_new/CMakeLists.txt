# add sources
target_sources(${PROJECT_NAME} PRIVATE
        sequence_system.cpp
        system_registry.cpp
        system_base.cpp
        composite_system.cpp
        system_factory.cpp
        system_executor.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        sequence_system.h
        system_handle.h
        system_registry.h
        system_base.h
        system_context.h
        composite_system.h
        system_factory.h
        system_executor.h
)
