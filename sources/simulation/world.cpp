#include "world.h"

#include "cell_factories/random_cell_factory.h"
#include "clock/clock.h"

#include "procedures/look_procedure_system.h"
#include "procedures/move_procedure_system.h"
#include "procedures/random_cell_spawn_procedure_system.h"

#include "simulation/simulation_statistics_provider.h"

#include "systems_ecs/graveyard_system.h"
#include "systems_ecs/keep_population_system.h"
#include "systems_ecs/spawn_places_statistics_system.h"

World::World()
{
    _simulationStorage.Store<EcsWorld>();

    const Common::Time targetSimulationTime = Common::Time::FromMilliseconds(30);
    _tickCalculator.Setup(targetSimulationTime);
}

// World::World(const SimulationConfig& config)
//     : _cellsLocator(config.cellsCountX, config.cellsCountY)
//     , _spawner(_ecsWorld, _cellsLocator)
//     , _simulationVm(_ecsWorld)
//     , _randomEngine(Random::MakeEngine("white"))
//     , _randomCellFactory(_simulationVm, _randomEngine)
//     , _statistics(_cellsLocator)
// {
//     const Common::Time targetSimulationTime = Common::Time::FromMilliseconds(30);
//
//     RegisterSystem<SpawnSystem>(_ecsWorld, _cellsLocator);
//     RegisterSystem<BrainSimulationSystem>(_ecsWorld, _simulationVm);
//     RegisterProcedureSystem<RandomCellSpawnProcedureSystem>(ProcedureType::SpawnRandomCell, 1, 0, "SpawnRandomCell", _ecsWorld, _simulationVm, _cellsLocator, _spawner, _randomCellFactory);
//     RegisterSystem<EnergyLeakSystem>(_ecsWorld);
//     RegisterSystem<EnergyDecreaseSystem>(_ecsWorld);
//     {
//         std::unique_ptr<SimulationSystem> system = RegisterAgeSystem(_simulationStorage);
//         _simulationSystems.push_back(std::move(system));
//     }
//     RegisterProcedureSystem<LookProcedureSystem>(ProcedureType::Look, 1, 1, "Look", _ecsWorld, _simulationVm, _cellsLocator);
//     RegisterProcedureSystem<MoveProcedureSystem>(ProcedureType::Move, 1, 0, "Move", _ecsWorld, _simulationVm, _cellsLocator);
//     RegisterSystem<AliveCellsStatisticsSystem>(_ecsWorld, _statistics);
//     RegisterSystem<SpawnPlacesStatisticsSystem>(_ecsWorld, _statistics);
//     RegisterSystem<DeathFromAgeStatisticsSystem>(_ecsWorld, _statistics);
//     RegisterSystem<DeathFromEmptyEnergySystem>(_ecsWorld, _statistics);
//     RegisterSystem<GraveyardSystem>(_ecsWorld, _cellsLocator);
//     {
//         KeepPopulationSystem::Config config {
//             &_ecsWorld,
//             &_cellsLocator,
//             &_spawner,
//             &_randomCellFactory,
//             &_statistics,
//             &_randomEngine
//         };
//         RegisterSystem<KeepPopulationSystem>(config);
//     }
//
//     _tickCalculator.Setup(targetSimulationTime);
// }

void World::AddSimulationSystem(std::unique_ptr<SimulationSystem> system)
{
    _simulationSystems.push_back(std::move(system));
}

void World::Update(const Common::Time elapsedTime)
{
    Warmup();

    const uint32_t ticks = _tickCalculator.CalculateElapsedTicks(GetTickTime(), elapsedTime);
    for (uint32_t i { 0 }; i < ticks; ++i) {
        Tick();
    }
}

void World::Warmup()
{
    while (!_tickSampler.IsFull()) {
        Tick();
    }
}

Common::Time World::GetTickTime() const
{
    return _tickSampler.CalcMedian();
}

void World::Tick()
{
    const Common::Clock clock;
    for (const auto& system : _simulationSystems) {
        system->DoSystemUpdate();
    }
    const Common::Time tickTime = clock.GetElapsedTime();
    _tickSampler.AddSample(tickTime);
}

// template <class T, class... Args>
//     requires std::is_base_of_v<ProcedureBase, T> && std::is_base_of_v<SimulationSystem, T> && std::is_constructible_v<T, Args...>
// T& World::RegisterProcedureSystem(ProcedureType type, uint8_t inputCount, uint8_t outputCount, std::string name, Args&&... args)
// {
//     auto procedure = std::make_unique<T>(std::forward<Args>(args)...);
//     T* weakProcedure = procedure.get();
//     _simulationVm.RegisterProcedure(type, weakProcedure, inputCount, outputCount, std::move(name));
//     _simulationSystems.push_back(std::move(procedure));
//     return *weakProcedure;
// }
//
// template <class T, class... Args>
//     requires std::is_base_of_v<SimulationSystem, T> && std::is_constructible_v<T, Args...>
// T& World::RegisterSystem(Args&&... args)
// {
//     auto& procedure = _simulationSystems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
//     return static_cast<T&>(*procedure);
// }