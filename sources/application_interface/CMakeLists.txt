project(cells-application-interface)
add_library(${PROJECT_NAME} STATIC stub.cpp)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        updatable.h
        drawable.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
        sfml-system
)