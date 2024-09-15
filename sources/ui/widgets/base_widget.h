#pragma once
#include "time/time.h"

class BaseWidget {
public:
    virtual ~BaseWidget() = default;
    virtual void UpdateWidget(Common::Time elapsedTime) = 0;
};