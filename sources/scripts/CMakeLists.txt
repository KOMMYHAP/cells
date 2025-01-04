project(cells-scripts)
add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE
        lua_registrable_system.cpp
        lua_system.cpp
        lua_system.h
        scripts_pch.h
)

target_sources(${PROJECT_NAME} PUBLIC
        lua_registrable_system.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        cells-common
        sol2::sol2
        lua
)
target_precompile_headers(${PROJECT_NAME} PRIVATE scripts_pch.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
