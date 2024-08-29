#include "world.h"

#include "SFML/Graphics/Shader.hpp"
#include "cell_factories/patrol_cell_factory.h"
#include "cell_factories/random_cell_factory.h"
#include "components/cell_type.h"
#include "procedures/look_procedure_system.h"
#include "procedures/move_procedure_system.h"
#include "procedures/random_cell_spawn_procedure_system.h"

#include "systems_ecs/brain_simulation_system.h"
#include "systems_ecs/spawn_system.h"

World::World()
    : _worldSize(100, 100)
    , _cellsLocator(_worldSize.x, _worldSize.y)
    , _spawner(_ecsWorld, _cellsLocator)
    , _simulationVm(_ecsWorld)
    , _randomEngine(Random::MakeEngine("white"))
    , _randomCellFactory(_simulationVm, _randomEngine)
{
    const sf::Time targetSimulationTime = sf::milliseconds(30);

    RegisterProcedureSystem<LookProcedureSystem>(ProcedureType::Look, 1, 1, "Look", _ecsWorld, _simulationVm, _cellsLocator);
    RegisterProcedureSystem<MoveProcedureSystem>(ProcedureType::Move, 1, 0, "Move", _ecsWorld, _simulationVm, _cellsLocator);
    RegisterProcedureSystem<RandomCellSpawnProcedureSystem>(ProcedureType::Reproduction, 1, 0, "SpawnRandomCell", _ecsWorld, _simulationVm, _cellsLocator, _spawner, _randomCellFactory);

    _simulationSystems.emplace_back(std::make_unique<BrainSimulationSystem>(_ecsWorld, _simulationVm));

    auto factory = [this](CellBrain& brain) {
        return _randomCellFactory.Make(brain);
    };
    static constexpr int CellsCount = 100;
    for (int i = 0; i < CellsCount; ++i) {
        const CellPosition position { NarrowCast<int16_t>(i % _worldSize.x), NarrowCast<int16_t>(i % _worldSize.y) };
        _spawner.TrySpawn(position, factory);
    }
    _tickCalculator.Setup(targetSimulationTime);
}

void World::Update(const sf::Time elapsedTime)
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

sf::Time World::GetTickTime() const
{
    return sf::seconds(_tickSampler.CalcMedian());
}

void World::Tick()
{
    const sf::Clock clock;
    for (const auto& system : _simulationSystems) {
        system->DoSystemUpdate();
    }
    const float seconds = clock.getElapsedTime().asSeconds();
    _tickSampler.AddSample(seconds);
}

template <class T, class... Args>
    requires std::is_base_of_v<ProcedureBase, T> && std::is_base_of_v<SimulationSystem, T> && std::is_constructible_v<T, Args...>
void World::RegisterProcedureSystem(ProcedureType type, uint8_t inputCount, uint8_t outputCount, std::string name, Args&&... args)
{
    auto procedure = std::make_unique<T>(std::forward<Args>(args)...);
    T* weakProcedure = procedure.get();
    _simulationVm.RegisterProcedure(type, weakProcedure, inputCount, outputCount, std::move(name));
    _simulationSystems.emplace_back(std::move(procedure));
}