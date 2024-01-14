project(cells-processor)
add_library(${PROJECT_NAME} STATIC)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        processor.cpp
        procedure.cpp
        procedure_table.cpp
        processor_control_block.cpp
        memory.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        processor.h
        procedure.h
        procedure_table.h
        processor_control_block.h
        memory.h
        memory.hpp
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC common)


# add pch
target_precompile_headers(${PROJECT_NAME} PRIVATE processor_pch.h)