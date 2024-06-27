#include "world.h"

#include "SFML/Graphics/Shader.hpp"
#include "cell_factories/preset_cell_factory.h"
#include "command_line.h"
#include "entt/entt.hpp"

#include "systems_ecs/movement_system.h"
#include "systems_ecs/spawn_system.h"

std::error_code World::InitializeSystem(ApplicationStorage& storage)
{
    _worldSize = {800, 600};

    _ecsWorld = std::make_unique<EcsWorld>();
    _simulation = std::make_unique<Simulation>();
    _cellFactory = std::make_unique<PresetCellFactory>();
    _currentPositions = std::make_unique<PositionSystem>(_worldSize.x, _worldSize.y);
    _nextPositions = std::make_unique<PositionSystem>(_worldSize.x, _worldSize.y);

    const sf::Time targetSimulationTime = sf::milliseconds(30);

    _simulationSystems.emplace_back(std::make_unique<MovementSystem>(*_ecsWorld, *_currentPositions, *_nextPositions));
    _simulationSystems.emplace_back(std::make_unique<SpawnSystem>(*_ecsWorld, *_cellFactory));

    _simulation->Setup(targetSimulationTime);

    storage.Store<World*>(this);

    return {};
}

void World::TerminateSystem()
{
    _cellFactory.reset();
    _nextPositions.reset();
    _currentPositions.reset();
    _simulationSystems.clear();
    _simulation.reset();
    _ecsWorld.reset();
}

void World::Update(sf::Time elapsedTime)
{
    const uint32_t ticks = _simulation->Run(elapsedTime);
    for (uint32_t i { 0 }; i < ticks; ++i) {
        for (const auto& system : _simulationSystems) {
            system->DoSystemUpdate();
        }
    }
}
