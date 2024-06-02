#pragma once

#include "basic_defines.h"

// GSL
#include <gsl/pointers>    // owner, not_null
#undef GSL_ASSUME
#undef GSL_LIKELY
#undef Expects
#undef Ensures