message(STATUS "Fetching gtest...")

FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG f8d7d77c06936315286eb55f8de22cd23c188571 # 1.14
)


set(BUILD_GMOCK OFF)
FetchContent_MakeAvailable(gtest)
