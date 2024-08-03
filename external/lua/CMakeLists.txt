project(lua)

add_library(${PROJECT_NAME} STATIC)

add_subdirectory(include)
add_subdirectory(src)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC include)