﻿message(STATUS "Fetching SDL...")

set(SDL_DISABLE_INSTALL ON)
set(SDL_TESTS OFF)
set(SDL_TEST_LIBRARY OFF)
set(SDL_SHARED OFF)
set(SDL_LIBC ON)
set(SDL_DISABLE_INSTALL_DOCS ON)
set(SDL_INSTALL_TESTS OFF)
set(SDL_ASAN ${ENABLE_ASAN})

FetchContent_Declare(SDL
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL"
    GIT_TAG "release-2.30.6"
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(SDL)