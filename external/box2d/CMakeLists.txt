message(STATUS "Fetching box2d...")

FetchContent_Declare(box2d
        GIT_REPOSITORY "https://github.com/erincatto/box2d.git"
        GIT_TAG "v3.1.1"
        GIT_SHALLOW ON
)

set(BOX2D_SAMPLES OFF)
set(BOX2D_BENCHMARKS OFF)
set(BOX2D_DOCS OFF)
set(BOX2D_PROFILE OFF)
set(BOX2D_VALIDATE OFF)
set(BOX2D_UNIT_TESTS OFF)
set(BOX2D_SANITIZE ${ENABLE_ASAN})

FetchContent_MakeAvailable(box2d)