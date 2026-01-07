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

void World::AddSimulationSystem(Common::Condition condition, std::unique_ptr<SimulationSystem> system)
{
    _systems.emplace_back(condition, std::move(system));
}

void World::Update(const Common::Time elapsedTime)
{
    const Common::Clock frameClock;
    if (_player.GetCurrentMode() == SimulationPlayer::Mode::FixedSpeed) {
        _player.UpdateElapsedTime(elapsedTime, _worldStatistics->GetTickTime());
    }
    _conditions.UpdateConditions();
    if (_player.ShouldPlayFrame()) {
        _player.PlayFrame();
    }
    SimulateStep();
    _worldStatistics->AddFrame(frameClock.GetElapsedTime());
}

void World::SimulateStep()
{
    for (const auto& data : _systems) {
        if (_conditions.Check(data.condition)) {
            data.system->DoSystemUpdate();
        }
    }
}