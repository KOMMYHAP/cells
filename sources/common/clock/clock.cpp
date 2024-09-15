#include "clock/clock.h"
#include "time/utils.h"

namespace Common {

Clock::Clock()
    : _time(GetCurrentPerformanceTime())
{
}

Time Clock::Restart()
{
    const auto now = GetCurrentPerformanceTime();
    const Time elapsedTime = now - _time;
    _time = now;
    return elapsedTime;
}

Time Clock::GetElapsedTime() const
{
    const auto now = GetCurrentPerformanceTime();
    return now - _time;
}

}