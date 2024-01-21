project(cell-tests)

add_executable(${PROJECT_NAME} stub.cpp tests_pch.h)

target_precompile_headers(${PROJECT_NAME} PRIVATE tests_pch.h)
target_link_libraries(${PROJECT_NAME} PRIVATE cells-processor cells-simulation gtest_main)