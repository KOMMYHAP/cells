#include "time/time.h"

#include "utils/narrow_cast.h"

namespace Common {

Time Time::FromSeconds(double seconds)
{
    const double us = seconds * 1000.0 * 1000.0;
#if defined(COMMON_ASSERTS_ENABLED)
    const int classification = std::fpclassify(us);
    ASSERT(classification == FP_ZERO || classification == FP_NORMAL, "overflow");
#endif
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

double Time::AsSeconds() const
{
    return static_cast<double>(_microseconds) / 1000.0 / 1000.0;
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

Time operator*(Time left, double right)
{
    return Time::FromSeconds(left.AsSeconds() * right);
}

Time operator*(double left, Time right)
{
    return right * left;
}

Time& operator*=(Time& left, double right)
{
    return left = left * right;
}

Time operator/(Time left, double right)
{
    ASSERT(right != 0.0f, "Invalid argument");
    return Time::FromSeconds(left.AsSeconds() / right);
}

Time& operator/=(Time& left, double right)
{
    return left = left / right;
}

double operator/(Time left, Time right)
{
    ASSERT(!right.IsZero(), "Invalid argument");
    return left.AsSeconds() / right.AsSeconds();
}

Time operator%(Time left, Time right)
{
    ASSERT(!right.IsZero(), "Invalid argument");
    return Time::FromMicroseconds(left.AsMicroseconds() % right.AsMicroseconds());
}

Time& operator%=(Time& left, Time right)
{
    return left = left % right;
}

}