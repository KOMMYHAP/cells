#pragma once

// #include <cassert>
// #if defined(assert)
// #undef assert
// #endif

#include "asserts_config.h"

// #include "libassert/assert.hpp"

#if defined(COMMON_ASSERTS_ENABLED)

#define ASSERT(x, ...) assert(!!(x))
#define ASSERT_FAIL(...) assert(false)
// #define ASSUME LIBASSERT_ASSUME

#else

#define ASSERT(...) (static_cast<void>(0))
#define ASSERT_FAIL(...) (static_cast<void>(0))
// #define ASSUME(...) (static_cast<void>(0))

#endif

#if defined(COMMON_ASSERTS_ENABLED)
#undef COMMON_ASSERTS_ENABLED
#endif