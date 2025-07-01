#include "world.h"

#include "clock/clock.h"
#include "world_statistics.h"

World::World(WorldStatistics& stats)
    : _worldStatistics(&stats)
{
    _simulationStorage.Store<EcsWorld>();

    const Common::Time targetSimulationTime = Common::Time::FromMilliseconds(30);
    _tickCalculator.Setup(targetSimulationTime);
}

void World::AddSimulationSystem(std::unique_ptr<SimulationSystem> system)
{
    _simulationSystems.push_back(std::move(system));
}

void World::Update(const Common::Time elapsedTime)
{
    const Common::Clock frameClock;
    int32_t ticks = 1;
    if (_worldStatistics->GetElapsedTicksCount() > 0) {
        ticks = _tickCalculator.CalculateElapsedTicks(GetTickTime(), elapsedTime);
    }
    for (int32_t i { 0 }; i < ticks; ++i) {
        Tick();
    }
    _worldStatistics->AddFrame(frameClock.GetElapsedTime());
}

Common::Time World::GetTickTime() const
{
    return _worldStatistics->GetTickTime();
}

void World::Tick()
{
    const Common::Clock tickClock;
    for (const auto& system : _simulationSystems) {
        system->DoSystemUpdate();
    }
    _worldStatistics->AddTick(tickClock.GetElapsedTime());
}