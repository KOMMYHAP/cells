#include "tick_calculator.h"

void SimulationTickCalculator::Setup(const Common::Time targetSimulationTime)
{
    _limitSimulationTime = targetSimulationTime;
}

uint32_t SimulationTickCalculator::CalculateElapsedTicks(const Common::Time tickTime, const Common::Time elapsedTime)
{
    if (tickTime.IsZero()) {
        return 0;
    }

    _availableTimeToSpent += elapsedTime;
    if (_availableTimeToSpent.AsSeconds() < 0.0f) {
        return 0;
    }

    const float availableTicks = _availableTimeToSpent / tickTime;
    constexpr float minimumTicks = 1.0f;
    const float maximumTicks = std::max(minimumTicks, _limitSimulationTime / tickTime);
    const float ticksToProcess = std::clamp(availableTicks, minimumTicks, maximumTicks);
    const uint32_t roundedTicksToProcess = static_cast<uint32_t>(ticksToProcess);

    _availableTimeToSpent -= tickTime * static_cast<float>(roundedTicksToProcess);
    _availableTimeToSpent = std::min(_availableTimeToSpent, tickTime);

    return roundedTicksToProcess;
}