include(FetchContent)

option(BUILD_SHARED_LIBS "Build external libraries as shared libraries?" ON)

add_subdirectory(pix)
add_subdirectory(gtest)
add_subdirectory(imgui)
add_subdirectory(sfml)
add_subdirectory(imgui-sfml)
add_subdirectory(libassert)