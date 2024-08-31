message(STATUS "Fetching entt...")

FetchContent_Declare(entt
    GIT_REPOSITORY "https://github.com/skypjack/entt"
    GIT_TAG "v3.13.2"
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(entt)