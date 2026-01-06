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

void World::AddSimulationSystem(Phase phase, std::unique_ptr<SimulationSystem> system)
{
    Systems& systems = _simulationSystems[phase];
    systems.push_back(std::move(system));
}

void World::Update(const Common::Time elapsedTime)
{
    const Common::Clock frameClock;
    if (_player.GetCurrentMode() == SimulationPlayer::Mode::FixedSpeed) {
        _player.UpdateElapsedTime(elapsedTime, _worldStatistics->GetTickTime());
    }
    while (_player.ShouldPlayFrame()) {
        _player.PlayFrame();
        Tick();
    }
    _worldStatistics->AddFrame(frameClock.GetElapsedTime());
}

void World::SetPhase(Phase phase)
{
    _activePhase = phase;
}

Common::Time World::GetTickTime() const
{
    return _worldStatistics->GetTickTime();
}

void World::Tick()
{
    const Common::Clock tickClock;
    for (const auto& system : _simulationSystems[_activePhase]) {
        system->DoSystemUpdate();
    }
    _worldStatistics->AddTick(tickClock.GetElapsedTime());
}