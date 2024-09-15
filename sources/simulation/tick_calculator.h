#pragma once
#include "time/time.h"

class SimulationTickCalculator {
public:
    void Setup(Common::Time targetSimulationTime);

    uint32_t CalculateElapsedTicks(Common::Time tickTime, Common::Time elapsedTime);

private:
    Common::Time _limitSimulationTime;
    Common::Time _availableTimeToSpent;
};