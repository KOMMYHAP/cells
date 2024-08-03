target_link_libraries(${PROJECT_NAME} PRIVATE pix)

# add sources
target_sources(${PROJECT_NAME} PRIVATE profile.cpp)


# add headers
target_sources(${PROJECT_NAME} PUBLIC profile.h)


# generate configuration header
option(COMMON_PROFILE_ENABLED "Enable profiling" OFF)
configure_file(profile_config.h.in profile_config.h @ONLY)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")


# add PIX profiler
target_link_libraries(${PROJECT_NAME} PRIVATE pix)