project(creatures)
add_executable(${PROJECT_NAME} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE
        main.cpp
        pch.h
)

target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        cells-common
        cells-ui
        EnTT::EnTT
        SDL3::SDL3
)
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)
target_copy_required_dll(${PROJECT_NAME})