#pragma once

#include "cell_factories/patrol_cell_factory.h"
#include "cell_factories/random_cell_factory.h"
#include "systems/brain_system.h"
#include "systems/graveyard_system.h"
#include "systems/health_system.h"
#include "systems/id_system.h"
#include "systems/position_system.h"
#include "systems/selection_system.h"
#include "systems/simulation_virtual_machine.h"
#include "systems/spawn_system.h"
#include "systems/type_system.h"
#include "world_interface.h"
#include "world_render.h"

class WorldWhite final : public WorldInterface {
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
    WorldWhite(Config&& config);

    void Tick() override;
    void Render(sf::RenderTarget& target, sf::RenderStates states);

    uint32_t GetCellsCount() const { return _idSystem.GetCellsCount(); }

private:
    static WorldRender::Config MakeRenderConfig(uint32_t cellSize, std::unique_ptr<sf::Shader> shader);
    SpawnSystem::Config MakeSpawnSystemConfig(float fullnessPercent);
    static SimulationVirtualMachine::Config MakeSimulationVmConfig(WorldWhite* world);

    void RegisterProcedures();
    void Respawn();

    IdSystem _idSystem;
    BrainSystem _brainSystem;
    TypeSystem _typeSystem;
    PositionSystem _positionSystem;
    GraveyardSystem _graveyardSystem;
    HealthSystem _healthSystem;
    SimulationVirtualMachine _simulationVm;
    SpawnSystem _spawnSystem;
    WorldRender _render;
    SelectionSystem _selectionSystem;

    PatrolCellFactory _patrolCellFactory;
    RandomCellFactory _randomCellFactory;
    ICellFactory* _defaultCellFactory{nullptr};
};