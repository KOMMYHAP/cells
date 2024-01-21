#include "simulation.h"
#include "brain/brain.h"
#include "brain/brain_processor.h"
#include "field/field.h"
#include "simulation_profile_category.h"

Simulation::Simulation(Field& field)
    : _field(field)
{
}

void Simulation::Tick()
{
    Ticks(1);
}

void Simulation::Ticks(uint32_t ticks)
{
    if (_tickCounter.IsFull()) {
        _tickCounter.Reset();
    }

    for (uint32_t i { 0 }; i < ticks; ++i) {
        ProcessTick();
    }
}

void Simulation::ProcessTick()
{
    common::ProfileScope tickProfileScope { "Tick", SimulationProfileCategory };
    sf::Clock clock;

    _field.IterateByData([this](const CellId id, Cell& cell) {
        Brain brain { cell };

        switch (brain.GetInfo().type) {
        case CellType::Unit: {
            BrainProcessor processor { id, brain, _field };
            processor.Process();
        } break;
        case CellType::Food:
            break;
        case CellType::Wall:
            break;
        case CellType::Dummy:
            break;
        }

        const auto& info = brain.GetInfo();
        if (info.type != CellType::Unit) {
            return;
        }
    });

    _tickCounter.AddSample(clock.getElapsedTime().asSeconds());
}

void Simulation::SetManualMode()
{
    _autoModeParams.reset();
}

void Simulation::SetAutoMode(float ticksPerSecond, sf::Time limitSimulationTime)
{
    assert(ticksPerSecond > 0.0f && limitSimulationTime.asSeconds() > 0.0f);
    _autoModeParams.emplace(ticksPerSecond, limitSimulationTime, 0.0f);
}

uint32_t Simulation::Run(sf::Time elapsedTime)
{
    if (!_autoModeParams.has_value()) {
        return 0;
    }

    RuntimeParams& params = *_autoModeParams;
    const sf::Time tickTime = GetTickTime();
    const bool tickTimeReady = _tickCounter.IsReady();

    const float elapsedTicks = tickTimeReady ? elapsedTime / tickTime : 1.0f;
    const float maximumElapsedTicks = params.ticksPerSecond * elapsedTime.asSeconds();
    params.ticksToProcess += std::min(elapsedTicks, maximumElapsedTicks);

    const float maximumTicks = tickTimeReady ? params.limitSimulationTime / tickTime : 1.0f;

    const float ticksToProcess = floor(std::min(params.ticksToProcess, maximumTicks));
    const uint32_t ticks = static_cast<uint32_t>(ticksToProcess);
    if (ticks == 0) {
        return 0;
    }

    Ticks(ticks);
    params.ticksToProcess -= ticksToProcess;
    return ticks;
}
