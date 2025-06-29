message(STATUS "Fetching sol2...")

FetchContent_Declare(
        sol2_content
        GIT_REPOSITORY https://github.com/ThePhD/sol2
        GIT_TAG 9190880c593dfb018ccf5cc9729ab87739709862 # 3.5.0
        GIT_SHALLOW ON
)

set(SOL2_SYSTEM_INCLUDE ON)
set(SOL2_ENABLE_INSTALL OFF)
set(SOL2_SINGLE ON)

FetchContent_MakeAvailable(sol2_content)