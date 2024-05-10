project(cells-common)
add_library(${PROJECT_NAME} STATIC)


option(ENABLE_ASAN "Enable ASAN?" OFF)
if (ENABLE_ASAN)
    target_asan_compile_options(${PROJECT_NAME})
endif ()


add_subdirectory(profile)
add_subdirectory(storage)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        common_pch.h
        flags.cpp
        command_line.cpp
        sample_counter.cpp
        breakpoint.cpp
        random.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        basic_defines.h
        flags.h
        command_line.h
        sample_counter.h
        sample_counter.hpp
        breakpoint.h
        random.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_precompile_headers(${PROJECT_NAME} PRIVATE common_pch.h)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
target_link_libraries(${PROJECT_NAME} PUBLIC
        libassert::assert
)
