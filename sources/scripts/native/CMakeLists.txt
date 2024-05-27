target_sources(${PROJECT_NAME} PRIVATE
        lua_system_factory.cpp
        lua_registrable_system.cpp
        lua_component_factory.cpp
        lua_system_impl.cpp
)

target_sources(${PROJECT_NAME} PUBLIC
        lua_system_factory.h
        lua_registrable_system.h
        lua_component_factory.h
        lua_system_impl.h
)