include(FetchContent)

option(BUILD_SHARED_LIBS "Build external libraries as shared libraries?" ON)

add_subdirectory(pix)
add_subdirectory(gtest)
add_subdirectory(imgui)
add_subdirectory(sfml)
add_subdirectory(imgui-sfml)
add_subdirectory(libassert)
add_subdirectory(nlohmann)
add_subdirectory(boost)
add_subdirectory(lua) # sol2 depends on lua, must be added before sol2
add_subdirectory(sol2)
add_subdirectory(gsl)
