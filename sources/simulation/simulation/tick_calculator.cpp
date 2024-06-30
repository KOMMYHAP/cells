#include "tick_calculator.h"

void SimulationTickCalculator::Setup(const sf::Time targetSimulationTime)
{
    ASSERT(targetSimulationTime.asSeconds() > 0.0f);

    _limitSimulationTime = targetSimulationTime;
    _availableTimeToSpent = sf::Time::Zero;
}

uint32_t SimulationTickCalculator::CalculateElapsedTicks(const sf::Time tickTime, const sf::Time elapsedTime)
{
    if (tickTime == sf::Time::Zero) {
        return 0;
    }

    _availableTimeToSpent += elapsedTime;
    if (_availableTimeToSpent.asSeconds() < 0.0f) {
        return 0;
    }

    const float availableTicks = _availableTimeToSpent / tickTime;
    constexpr float minimumTicks = 1.0f;
    const float maximumTicks = std::max(minimumTicks, _limitSimulationTime / tickTime);
    const float ticksToProcess = std::clamp(availableTicks, minimumTicks, maximumTicks);
    const uint32_t roundedTicksToProcess = static_cast<uint32_t>(ticksToProcess);

    _availableTimeToSpent -= sf::seconds(static_cast<float>(roundedTicksToProcess) * tickTime.asSeconds());
    _availableTimeToSpent = std::min(_availableTimeToSpent, tickTime);

    return roundedTicksToProcess;
}