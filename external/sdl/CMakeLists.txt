message(STATUS "Fetching SDL...")

FetchContent_Declare(SDL
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL"
    GIT_TAG "release-2.30.6"
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(SDL)