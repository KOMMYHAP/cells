add_subdirectory(itt)
add_subdirectory(pix)

set(BUILD_GMOCK OFF)
set(INSTALL_GTEST OFF)
set(GTEST_HAS_ABSL OFF)
set(gtest_force_shared_crt ON)
add_subdirectory(googletest)