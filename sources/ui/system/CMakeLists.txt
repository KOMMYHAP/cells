target_sources(${PROJECT_NAME} PRIVATE
        app_statistics.cpp
        app_statistics.h
        sdl_panic.cpp
        sdl_panic.h
        ui_pch.h
        ui_system.cpp
        ui_system.h
)
target_precompile_headers(${PROJECT_NAME} PRIVATE ui_pch.h)