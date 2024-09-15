#pragma once
#include "time/time.h"

namespace Common {

class Clock {
public:
    Clock();
    Time Restart();
    Time GetElapsedTime() const;

private:
    Time _time;
};

}