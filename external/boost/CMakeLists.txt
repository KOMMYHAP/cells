message(STATUS "Fetching boost...")
# https://github.com/boostorg/cmake

FetchContent_Declare(
        boost
        GIT_REPOSITORY https://github.com/boostorg/boost
        GIT_TAG ab7968a0bbcf574a7859240d1d8443f58ed6f6cf # 1.85
)


# A semicolon-separated list of libraries to include into the build (and installation.)
# Defaults to empty, which means "all libraries". Example: filesystem;regex.
set(BOOST_INCLUDE_LIBRARIES any)

# A semicolon-separated list of libraries to exclude from the build (and installation.)
# This is useful if a library causes an error in the CMake configure phase.
set(BOOST_EXCLUDE_LIBRARIES "")

set(BOOST_ENABLE_MPI OFF)
set(BOOST_ENABLE_PYTHON OFF)
FetchContent_MakeAvailable(boost)
