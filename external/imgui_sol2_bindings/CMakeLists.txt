# Dear ImPlot
message(STATUS "Fetching ImGui-Sol2 bindings ...")
FetchContent_Declare(
        imgui-sol2-bindings
        GIT_REPOSITORY https://github.com/Fesmaster/sol2_ImGui_Bindings
        GIT_TAG ab4ebe870d74da96337eee6dd6a645d39aa1b3c5
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(imgui-sol2-bindings)