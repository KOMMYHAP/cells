include(cmake/target_asan_compile_options.cmake)
include(cmake/target_copy_required_dll.cmake)

add_subdirectory(common)
add_subdirectory(processor)
add_subdirectory(simulation)
add_subdirectory(tests)
add_subdirectory(application)
add_subdirectory(ui)
