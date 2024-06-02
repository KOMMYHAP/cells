message(STATUS "Fetching ImGui-SFML...")

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)

# ImGui-SFML
FetchContent_Declare(
        imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml
        GIT_TAG de565ac8f2b795dedc0307b60830cb006afd2ecd # 2.6
        GIT_SHALLOW ON
)

FetchContent_MakeAvailable(imgui-sfml)