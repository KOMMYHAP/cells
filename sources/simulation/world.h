#pragma once

#include "cell_factories/patrol_cell_factory.h"
#include "cell_factories/random_cell_factory.h"
#include "systems/age_system.h"
#include "systems/brain_system.h"
#include "systems/graveyard_system.h"
#include "systems/health_system.h"
#include "systems/id_system.h"
#include "systems/position_system.h"
#include "systems/selection_system.h"
#include "systems/simulation_virtual_machine.h"
#include "systems/spawn_system.h"
#include "systems/spawner.h"
#include "systems/type_system.h"
#include "world_render.h"
#include "updatable.h"

class World {
public:
    enum class SpawnPolicy {
        Random,
        Patrol
    };
    struct Config {
        uint32_t width;
        uint32_t height;
        uint32_t cellSize;
        uint8_t fullnessPercent;
        std::unique_ptr<sf::Shader> shader;
        SpawnPolicy spawnPolicy { SpawnPolicy::Patrol };
    };
    World(Config&& config);

    void Update(sf::Time elapsedTime);
    void Render(sf::RenderTarget& target, sf::RenderStates states);

    struct Statistics {
        uint32_t cellsCount { 0 };
        uint32_t generation { 0 };
        uint64_t tick { 0 };
    };
    Statistics GetStatistics() const { return _statistics; }

private:
    static WorldRender::Config MakeRenderConfig(uint32_t cellSize, std::unique_ptr<sf::Shader> shader);
    SpawnSystem::Config MakeSpawnSystemConfig(float fullnessPercent);
    static SimulationVirtualMachine::Config MakeSimulationVmConfig(World* world);

    void RegisterProcedures();

    void SpawnMoreIfNeeded();
    void Respawn();

    IdSystem _idSystem;
    BrainSystem _brainSystem;
    TypeSystem _typeSystem;
    PositionSystem _positionSystem;
    GraveyardSystem _graveyardSystem;
    HealthSystem _healthSystem;
    SimulationVirtualMachine _simulationVm;
    Spawner _spawner;
    SpawnSystem _spawnSystem;
    WorldRender _render;
    SelectionSystem _selectionSystem;
    AgeSystem _ageSystem;

    PatrolCellFactory _patrolCellFactory;
    RandomCellFactory _randomCellFactory;
    ICellFactory* _defaultCellFactory { nullptr };

    Statistics _statistics;
};