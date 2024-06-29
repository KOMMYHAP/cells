#include "tick_calculator.h"

void SimulationTickCalculator::Setup(sf::Time targetSimulationTime)
{
    ASSERT(targetSimulationTime.asSeconds() > 0.0f);

    _limitSimulationTime = targetSimulationTime;
    _availableTimeToSpent = sf::Time::Zero;
    _tickTime = sf::seconds(1);
}

uint32_t SimulationTickCalculator::Run(sf::Time elapsedTime)
{
    if (_tickTime == sf::Time::Zero) {
        return 0;
    }

    _availableTimeToSpent += elapsedTime;
    if (_availableTimeToSpent.asSeconds() < 0.0f) {
        return 0;
    }

    const float availableTicks = _availableTimeToSpent / _tickTime;
    constexpr float minimumTicks = 1.0f;
    const float maximumTicks = std::max(minimumTicks, _limitSimulationTime / _tickTime);
    const float ticksToProcess = std::clamp(availableTicks, minimumTicks, maximumTicks);
    const uint32_t roundedTicksToProcess = static_cast<uint32_t>(ticksToProcess);

    _availableTimeToSpent -= sf::seconds(static_cast<float>(roundedTicksToProcess) * _tickTime.asSeconds());
    _availableTimeToSpent = std::min(_availableTimeToSpent, _tickTime);

    return roundedTicksToProcess;
}