include(FetchContent)

option(BUILD_SHARED_LIBS "Build external libraries as shared libraries?" OFF)

add_subdirectory(pix)
add_subdirectory(gtest)
# add_subdirectory(libassert)
# add_subdirectory(nlohmann)
add_subdirectory(gsl)
add_subdirectory(entt)
add_subdirectory(sdl)
add_subdirectory(imgui) # imgui depends on SDL2 backend
