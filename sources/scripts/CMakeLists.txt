project(cells-scripts)
add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE
        lua_registrable_system.cpp
)

target_sources(${PROJECT_NAME} PUBLIC
        lua_registrable_system.h
)

add_subdirectory(system)
add_subdirectory(bind)

target_link_libraries(${PROJECT_NAME} PRIVATE
        sol2::sol2
        lua
        ImGuiLib
        cells-common
        cells-simulation
        cells-ui
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
