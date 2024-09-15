#include "time/utils.h"

#include "utils/narrow_cast.h"

#include <chrono>

namespace Common {

Time GetCurrentTime()
{
    const auto now = std::chrono::system_clock::now().time_since_epoch();
    const auto us = std::chrono::duration_cast<std::chrono::microseconds>(now).count();
    return Time::FromMicroseconds(NarrowCast<uint64_t>(us));
}

Time GetCurrentPerformanceTime()
{
    const auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    const auto us = std::chrono::duration_cast<std::chrono::microseconds>(now).count();
    return Time::FromMicroseconds(NarrowCast<uint64_t>(us));
}

}