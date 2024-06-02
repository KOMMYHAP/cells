message(STATUS "Fetching nlohmann JSON...")

FetchContent_Declare(
        nlohmann
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG 9cca280a4d0ccf0c08f47a99aa71d1b0e52f8d03 # 3.11.3
        GIT_SHALLOW ON
)


#target_link_libraries(foo PRIVATE nlohmann_json::nlohmann_json)

FetchContent_MakeAvailable(nlohmann)
