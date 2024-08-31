project(cell-tests)

add_executable(${PROJECT_NAME} main_tests.cpp tests_pch.h
        processor_stack_tests.cpp
        processor_memory_tests.cpp
        processor_tests.cpp
        procedure_tests.cpp
        storage_tests.cpp
        stack_storage_tests.cpp
        registrar_tests.cpp
        component_storage_tests.cpp
        system_tests.cpp
        #sol2_tests.cpp
        ecs_tests.cpp
)

add_subdirectory(utils)

target_precompile_headers(${PROJECT_NAME} PRIVATE tests_pch.h)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    cells-processor 
    cells-simulation 
    #cells-scripts 
    gtest
    #sol2::sol2
    #lua 
    EnTT::EnTT
)
target_copy_required_dll(${PROJECT_NAME})