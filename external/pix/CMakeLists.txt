project(pix)

add_library(${PROJECT_NAME} SHARED IMPORTED GLOBAL)
set_property(TARGET ${PROJECT_NAME} PROPERTY IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/WinPixEventRuntime.lib")
target_include_directories(${PROJECT_NAME} INTERFACE include)

option(EXTERNAL_PIX_ENABLED "Enable PIX profiler" OFF)
if (EXTERNAL_PIX_ENABLED)
    target_compile_definitions(${PROJECT_NAME} INTERFACE USE_PIX)
    set_property(TARGET ${PROJECT_NAME} PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/bin/WinPixEventRuntime.dll")
endif ()