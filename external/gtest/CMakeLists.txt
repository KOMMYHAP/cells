message(STATUS "Fetching gtest...")

FetchContent_Declare(
        gtest_patched
        GIT_REPOSITORY https://github.com/KOMMYHAP/googletest
        GIT_TAG d175af9ea3a0055fd29deb18ec0af25a54eeceb1 # 1.14.0_rc_patch
        GIT_SHALLOW ON
)


set(gtest_force_shared_crt ${BUILD_SHARED_LIBS})
set(gtest_enable_system_include OFF)
set(BUILD_GMOCK OFF)
set(INSTALL_GTEST OFF)
FetchContent_MakeAvailable(gtest_patched)
