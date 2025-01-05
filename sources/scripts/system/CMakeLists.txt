target_sources(${PROJECT_NAME} PRIVATE
        lua_system.cpp
        lua_system.h
        lua_logger.h
        lua_logger.hpp
        lua_logger.cpp
        scripts_pch.h
)

target_precompile_headers(${PROJECT_NAME} PRIVATE scripts_pch.h)
