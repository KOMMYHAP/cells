message(STATUS "Fetching libassert...")

FetchContent_Declare(
        libassert
        GIT_REPOSITORY https://github.com/jeremy-rifkin/libassert
        GIT_TAG e64e4dda62bdd0a07ee97ae54b6c539cd412ef34 # 2.0.1
        GIT_SHALLOW ON
)

FetchContent_MakeAvailable(libassert)