message(STATUS "Fetching ImGui...")

# Dear ImGui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG 231cbee0fc4f59dbe5b8b853a11b08dc84e57c65 # 1.90.5
)


FetchContent_MakeAvailable(imgui)

set(IMGUI_DIR ${FETCHCONTENT_BASE_DIR}/imgui-src CACHE PATH "Source directory of Dear ImGui fetched by cmake")