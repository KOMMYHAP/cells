#include "spin_lock.h"

static_assert(std::atomic_bool::is_always_lock_free, "Spin lock requires lock free atomic operation!");