# Dear ImGui
message(STATUS "Fetching ImGui...")
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG 231cbee0fc4f59dbe5b8b853a11b08dc84e57c65 # 1.90.5
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR ${FETCHCONTENT_BASE_DIR}/imgui-src CACHE PATH "Source directory of Dear ImGui fetched by cmake")


# Dear ImPlot
message(STATUS "Fetching ImPlot...")
FetchContent_Declare(
        implot
        GIT_REPOSITORY https://github.com/epezent/implot
        GIT_TAG "v0.16"
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(implot)
set(IMPLOT_DIR ${FETCHCONTENT_BASE_DIR}/implot-src CACHE PATH "Source directory of ImPlot fetched by cmake")


# Combine ImGuiLib from ImGui + ImPlot
project(ImGuiLib)
add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PRIVATE
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_internal.h
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer2.cpp
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_demo.cpp
    ${IMPLOT_DIR}/implot_items.cpp
    ${IMPLOT_DIR}/implot_internal.h
)
target_sources(${PROJECT_NAME} PUBLIC
    ${IMGUI_DIR}/imconfig.h
    ${IMGUI_DIR}/imgui.h
    ${IMGUI_DIR}/misc/cpp/imgui_stdlib.h
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.h
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer2.h
    ${IMPLOT_DIR}/implot.h
)
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)