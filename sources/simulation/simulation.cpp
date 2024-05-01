#include "simulation.h"
#include "simulation_profile_category.h"
#include "world_interface.h"

Simulation::Simulation(World& world)
    : _world(world)
{
}

void Simulation::Tick()
{
    Ticks(1);
}

void Simulation::Ticks(uint32_t ticks)
{
    for (uint32_t i { 0 }; i < ticks; ++i) {
        ProcessTick();
    }
}

void Simulation::ProcessTick()
{
    common::ProfileScope tickProfileScope { "TickGeneration", SimulationProfileCategory };
    sf::Clock clock;
    _world.Tick();
    _tickCounter.AddSample(clock.getElapsedTime().asSeconds());
}

void Simulation::SetManualMode()
{
    _autoModeParams.reset();
}

void Simulation::SetAutoMode(sf::Time targetSimulationTime)
{
    ASSERT(targetSimulationTime.asSeconds() > 0.0f);
    _autoModeParams.emplace(targetSimulationTime, sf::Time::Zero);
}

uint32_t Simulation::Run(sf::Time elapsedTime)
{
    if (!_autoModeParams.has_value()) {
        return 0;
    }
    if (!_tickCounter.IsFull()) {
        return WarmUp();
    }

    RuntimeParams& params = *_autoModeParams;

    params.availableTimeToSpent += elapsedTime;
    if (params.availableTimeToSpent.asSeconds() < 0.0f) {
        return 0;
    }

    const sf::Time tickTime = sf::seconds(_tickCounter.CalcMedian());
    const float availableTicks = params.availableTimeToSpent / tickTime;
    const float minimumTicks = 1.0f;
    const float maximumTicks = std::max(minimumTicks, params.limitSimulationTime / tickTime);
    const float ticksToProcess = std::clamp(availableTicks, minimumTicks, maximumTicks);
    const uint32_t roundedTicksToProcess = static_cast<uint32_t>(ticksToProcess);

    params.availableTimeToSpent -= sf::seconds(roundedTicksToProcess * tickTime.asSeconds());
    params.availableTimeToSpent = std::min(params.availableTimeToSpent, tickTime);

    Ticks(roundedTicksToProcess);
    return roundedTicksToProcess;
}

uint32_t Simulation::WarmUp()
{
    const uint32_t ticksToWarmUp = 1;
    Ticks(ticksToWarmUp);
    return ticksToWarmUp;
}

void Simulation::Update(sf::Time elapsedTime)
{
    Run(elapsedTime);
}
