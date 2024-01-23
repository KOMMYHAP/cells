project(cell-tests)

add_executable(${PROJECT_NAME} main_tests.cpp tests_pch.h
        processor_stack_tests.cpp
        processor_memory_tests.cpp
        processor_tests.cpp
        procedure_tests.cpp
)

target_precompile_headers(${PROJECT_NAME} PRIVATE tests_pch.h)
target_link_libraries(${PROJECT_NAME} PRIVATE cells-processor cells-simulation gtest)

# copy all .dll near .exe
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
)
