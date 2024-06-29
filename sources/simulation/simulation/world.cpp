#include "world.h"

#include "SFML/Graphics/Shader.hpp"
#include "entt/entt.hpp"

#include "systems_ecs/movement_system.h"
#include "systems_ecs/spawn_system.h"

World::World()
    : _worldSize(800, 600)
    , _currentPositions(_worldSize.x, _worldSize.y)
    , _nextPositions(_worldSize.x, _worldSize.y)
{
    const sf::Time targetSimulationTime = sf::milliseconds(30);

    _simulationSystems.emplace_back(std::make_unique<MovementSystem>(_ecsWorld, _currentPositions, _nextPositions));
    _simulationSystems.emplace_back(std::make_unique<SpawnSystem>(_ecsWorld, Random::Accessor { _randomEngine }, _simulationVm));

    _tickCalculator.Setup(targetSimulationTime);
}

void World::Update(const sf::Time elapsedTime)
{
    const uint32_t ticks = _tickCalculator.Run(elapsedTime);
    for (uint32_t i { 0 }; i < ticks; ++i) {
        for (const auto& system : _simulationSystems) {
            system->DoSystemUpdate();
        }
    }
}
