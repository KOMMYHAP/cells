#include "world_white.h"

#include "procedures/move_procedure.h"

WorldWhite::WorldWhite(Config&& config)
    : _idSystem(config.width * config.height)
    , _brainSystem(_idSystem.GetCellsCountLimit())
    , _typeSystem(_idSystem.GetCellsCountLimit())
    , _positionSystem(config.width, config.height)
    , _graveyardSystem(_idSystem.GetCellsCountLimit(), _idSystem, _typeSystem, _positionSystem)
    , _healthSystem(_idSystem.GetCellsCountLimit(), _graveyardSystem)
    , _simulationVm(MakeSimulationVmConfig(this))
    , _spawnSystem(MakeSpawnSystemConfig(config.fullnessPercent))
    , _render(MakeRenderConfig(config.cellSize, std::move(config.shader)), _positionSystem, _idSystem, _typeSystem)
    , _selectionSystem(_brainSystem, _idSystem, 500, 10)
    , _patrolCellFactory(_simulationVm, 10)
    , _randomCellFactory()
{
    RegisterProcedures();

    switch (config.spawnPolicy) {
    case SpawnPolicy::Random:
        _defaultCellFactory = &_randomCellFactory;
        break;
    case SpawnPolicy::Patrol:
        _defaultCellFactory = &_patrolCellFactory;
        break;
    default:
        assert(false);
    }
}

void WorldWhite::Tick()
{
    // Process brain of each cell.
    _idSystem.Iterate([this](const CellId id) {
        _simulationVm.Run(id);
    });

    // Cleanup world systems from dead cells.
    _graveyardSystem.Cleanup();

    // Spawn more if needed
    const uint32_t aliveCellsCount = _idSystem.GetCellsCount();
    if (aliveCellsCount == 0) {
        _selectionSystem.Restart();
        _spawnSystem.SetCellFactory(*_defaultCellFactory);
        Respawn();
    } else {
        SelectionSystem::Result selectionResult = _selectionSystem.TickGeneration();
        if (selectionResult.shouldRespawn) {
            _spawnSystem.SetCellFactory(*selectionResult.cellFactory);
            Respawn();
        }
    }
}

void WorldWhite::RegisterProcedures()
{
    _simulationVm.RegisterProcedure<MoveProcedure>(ProcedureType::Move, 1, 0, "move", _simulationVm, _positionSystem);
}

WorldRender::Config WorldWhite::MakeRenderConfig(uint32_t cellSize, std::unique_ptr<sf::Shader> shader)
{
    assert(cellSize < 255);
    const sf::Color gray { 0xCCCCCCFF };
    return {
        std::move(shader),
        { sf::Color::Magenta, sf::Color::Green, sf::Color::Black, gray },
        static_cast<uint8_t>(cellSize)
    };
}

void WorldWhite::Render(sf::RenderTarget& target, sf::RenderStates states)
{
    _render.Render(target, states);
}

SimulationVirtualMachine::Config WorldWhite::MakeSimulationVmConfig(WorldWhite* world)
{
    auto watcher = [world](ProcessorState state) {
        if (state == ProcessorState::Good) {
            return;
        }

        // Cell's brain has illegal instruction, make insult as punishment
        const CellId id = world->_simulationVm.GetRunningCellId();
        world->_healthSystem.Set(id, CellHealth::Zero);
    };
    return {
        world->_brainSystem,
        world->_typeSystem,
        world->_healthSystem,
        8,
        std::move(watcher)
    };
}

SpawnSystem::Config WorldWhite::MakeSpawnSystemConfig(float fullnessPercent)
{
    const auto targetPopulationSize = static_cast<uint32_t>(round(fullnessPercent * static_cast<float>(_idSystem.GetCellsCountLimit()) / 100.0f));
    return {
        _positionSystem,
        _idSystem,
        _typeSystem,
        _brainSystem,
        targetPopulationSize,
    };
}

void WorldWhite::Respawn()
{
    _idSystem.Iterate([&](const CellId id) {
        _graveyardSystem.Bury(id);
    });
    _graveyardSystem.Cleanup();

    _spawnSystem.TryToSpawn();
}
