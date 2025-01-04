target_sources(${PROJECT_NAME} PRIVATE
        sdl_panic.cpp
        sdl_panic.h
        ui_pch.h
        ui_system.cpp
        ui_system.h
)
target_precompile_headers(${PROJECT_NAME} PRIVATE ui_pch.h)