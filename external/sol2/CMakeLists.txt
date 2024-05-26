message(STATUS "Fetching sol2...")

FetchContent_Declare(
        sol2_content
        GIT_REPOSITORY https://github.com/ThePhD/sol2
        GIT_TAG dca62a0f02bb45f3de296de3ce00b1275eb34c25 # 3.3.1
)

set(SOL2_SYSTEM_INCLUDE ON)
set(SOL2_ENABLE_INSTALL OFF)
set(SOL2_SINGLE ON)

FetchContent_MakeAvailable(sol2_content)