target_sources(${PROJECT_NAME} PRIVATE
        sdl_panic.cpp
        sdl_panic.h
        ui_pch.h
        ui_system.cpp
        ui_system.h
)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ui_pch.h)