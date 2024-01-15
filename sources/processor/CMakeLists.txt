project(cells-processor)
add_library(${PROJECT_NAME} STATIC)


add_subdirectory(procedures)
add_subdirectory(processor)
add_subdirectory(vm)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC common)

target_sources(${PROJECT_NAME} PUBLIC processor_profile_category.h)


target_precompile_headers(${PROJECT_NAME} PRIVATE processor_pch.h)