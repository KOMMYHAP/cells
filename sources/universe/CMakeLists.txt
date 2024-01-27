project(cells-universe)
add_executable(${PROJECT_NAME} main.cpp)


# find SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1 EXACT REQUIRED system)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        world_white.cpp
        main.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        world_white.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
        cells-common
        cells-processor
        cells-render
        cells-simulation
        sfml-system
)


# add pch
target_precompile_headers(${PROJECT_NAME} PRIVATE universe_pch.h)


# copy all .dll near .exe
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
)
