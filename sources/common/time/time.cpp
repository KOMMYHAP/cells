#include "time/time.h"

#include "utils/narrow_cast.h"

namespace Common {

Time Time::FromSeconds(float seconds)
{
    const float us = seconds * 1000.0f * 1000.0f;
    const int classification = std::fpclassify(us);
    ASSERT(classification == FP_ZERO || classification == FP_NORMAL, "overflow");
    const int64_t roundedMicroseconds = std::llround(us);
    return Time { roundedMicroseconds };
}

Time Time::FromMilliseconds(int64_t ms)
{
    ASSERT(ms <= std::numeric_limits<int64_t>::max() / 1000, "overflow");
    return Time { ms * 1000ll };
}

Time Time::FromMicroseconds(int64_t us)
{
    return Time { us };
}

float Time::AsSeconds() const
{
    return static_cast<float>(_microseconds) / 1000.0f / 1000.0f;
}

int64_t Time::AsMilliseconds() const
{
    return NarrowCast<int64_t>(_microseconds / 1000);
}

int64_t Time::AsMicroseconds() const
{
    return _microseconds;
}

bool Time::IsZero() const
{
    return _microseconds == 0;
}

Time::Time(int64_t us)
    : _microseconds(us)
{
}

bool operator==(Time left, Time right)
{
    return left.AsMicroseconds() == right.AsMicroseconds();
}

bool operator!=(Time left, Time right)
{
    return left.AsMicroseconds() != right.AsMicroseconds();
}

bool operator<(Time left, Time right)
{
    return left.AsMicroseconds() < right.AsMicroseconds();
}

bool operator>(Time left, Time right)
{
    return left.AsMicroseconds() > right.AsMicroseconds();
}

bool operator<=(Time left, Time right)
{
    return left.AsMicroseconds() <= right.AsMicroseconds();
}

bool operator>=(Time left, Time right)
{
    return left.AsMicroseconds() >= right.AsMicroseconds();
}

Time operator-(Time right)
{
    return Time::FromMicroseconds(-right.AsMicroseconds());
}

Time operator+(Time left, Time right)
{
    return Time::FromMicroseconds(left.AsMicroseconds() + right.AsMicroseconds());
}

Time& operator+=(Time& left, Time right)
{
    return left = left + right;
}

Time operator-(Time left, Time right)
{
    return Time::FromMicroseconds(left.AsMicroseconds() - right.AsMicroseconds());
}

Time& operator-=(Time& left, Time right)
{
    return left = left - right;
}

Time operator*(Time left, float right)
{
    return Time::FromSeconds(left.AsSeconds() * right);
}

Time operator*(float left, Time right)
{
    return right * left;
}

Time& operator*=(Time& left, float right)
{
    return left = left * right;
}

Time operator/(Time left, float right)
{
    ASSERT(right != 0.0f);
    return Time::FromSeconds(left.AsSeconds() / right);
}

Time& operator/=(Time& left, float right)
{
    return left = left / right;
}

float operator/(Time left, Time right)
{
    ASSERT(!right.IsZero());
    return left.AsSeconds() / right.AsSeconds();
}

Time operator%(Time left, Time right)
{
    ASSERT(!right.IsZero());
    return Time::FromMicroseconds(left.AsMicroseconds() % right.AsMicroseconds());
}

Time& operator%=(Time& left, Time right)
{
    return left = left % right;
}

}