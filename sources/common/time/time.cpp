#include "time.h"

#include "utils/narrow_cast.h"

namespace Common {

Time Time::FromSeconds(float seconds)
{
    ASSERT(seconds >= 0.0f, "negative time");
    const float us = seconds * 1000.0f * 1000.0f;
    const int classification = std::fpclassify(us);
    ASSERT(classification == FP_ZERO || classification == FP_NORMAL, "overflow");
    const int64_t roundedMicroseconds = std::llround(us);
    return Time { NarrowCast<uint64_t>(roundedMicroseconds) };
}

Time::Time(uint64_t microseconds)
    : _microseconds(microseconds)
{
}

Time Time::FromMilliseconds(uint64_t ms)
{
    ASSERT(ms <= std::numeric_limits<uint64_t>::max() / 1000, "overflow");
    return Time { ms * 1000ull };
}

Time Time::FromMicroseconds(uint64_t us)
{
    return Time { us };
}

float Time::AsSeconds() const
{
    return static_cast<float>(_microseconds) / 1000.0f / 1000.0f;
}

uint64_t Time::AsMilliseconds() const
{
    return NarrowCast<uint32_t>(_microseconds / 1000);
}

uint64_t Time::AsMicroseconds() const
{
    return _microseconds;
}

}