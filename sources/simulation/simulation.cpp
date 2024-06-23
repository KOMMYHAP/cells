#include "simulation.h"

void Simulation::Setup(sf::Time targetSimulationTime, std::vector<SimulationSystem*> systems)
{
    ASSERT(targetSimulationTime.asSeconds() > 0.0f);

    _systems = std::move(systems);
    _limitSimulationTime = targetSimulationTime;
    _availableTimeToSpent = sf::Time::Zero;
}

uint32_t Simulation::Run(sf::Time elapsedTime)
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

    Ticks(roundedTicksToProcess);
    return roundedTicksToProcess;
}

void Simulation::Ticks(uint32_t count)
{
    for (auto _ : std::ranges::iota_view { 0u, count }) {
        for (const auto& system : _systems) {
            system->Update();
        }
    }
}
