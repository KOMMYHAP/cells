project(cells-common)
add_library(${PROJECT_NAME} STATIC)


option(ENABLE_ASAN "Enable ASAN?" OFF)
if (ENABLE_ASAN)
    target_asan_compile_options(${PROJECT_NAME})
endif ()


add_subdirectory(profile)
add_subdirectory(storage)
add_subdirectory(registrar)
add_subdirectory(utils)
add_subdirectory(asserts)
add_subdirectory(mt)
add_subdirectory(random)


# add sources
target_sources(${PROJECT_NAME} PRIVATE
        flags.cpp
        command_line.cpp
        sample_counter.cpp
        breakpoint.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        basic_defines.h
        flags.h
        command_line.h
        sample_counter.h
        sample_counter.hpp
        breakpoint.h
        common.h
        application_storage.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_precompile_headers(${PROJECT_NAME} PRIVATE basic_defines.h)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
target_link_libraries(${PROJECT_NAME} PUBLIC
        Microsoft.GSL::GSL
)

if (MSVC)
    # target_compile_options(${PROJECT_NAME} PUBLIC /WX /W4 /permissive-)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    # target_compile_options(${PROJECT_NAME} PUBLIC -Werror -Wall -Wextra -Wpedantic)
endif ()
