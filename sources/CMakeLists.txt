add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE main.cpp pch.h)
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)