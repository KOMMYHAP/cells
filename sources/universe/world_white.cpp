#include "world_white.h"

#include "procedures/consume_procedure.h"
#include "procedures/look_procedure.h"
#include "procedures/move_procedure.h"
#include "procedures/reproduction_procedure.h"

constexpr CellAge LimitCellAge { 100 };
constexpr uint16_t BestCellSelectionSize { 10 };
constexpr uint16_t SelectionEpochTicks { 1000 };

WorldWhite::WorldWhite(Config&& config)
    : _idSystem(config.width * config.height)
    , _brainSystem(_idSystem.GetCellsCountLimit())
    , _typeSystem(_idSystem.GetCellsCountLimit())
    , _positionSystem(config.width, config.height)
    , _graveyardSystem(_idSystem.GetCellsCountLimit(), _idSystem, _typeSystem, _positionSystem)
    , _healthSystem(_idSystem.GetCellsCountLimit(), _graveyardSystem)
    , _simulationVm(MakeSimulationVmConfig(this))
    , _spawner(_positionSystem, _typeSystem, _brainSystem, _healthSystem, _ageSystem, _idSystem)
    , _spawnSystem(MakeSpawnSystemConfig(config.fullnessPercent))
    , _render(MakeRenderConfig(config.cellSize, std::move(config.shader)), _positionSystem, _idSystem, _typeSystem)
    , _selectionSystem(_brainSystem, _idSystem, SelectionEpochTicks, BestCellSelectionSize)
    , _patrolCellFactory(_simulationVm, 10)
    , _randomCellFactory(_simulationVm, std::optional<uint16_t>())
    , _ageSystem(_idSystem.GetCellsCountLimit(), _healthSystem)
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
        UNREACHABLE("Unknown spawn policy!", config.spawnPolicy);
    }
}

void WorldWhite::Tick()
{
    // Process brain of each cell.
    _idSystem.Iterate([this](const CellId id) {
        _simulationVm.Run(id);
    });

    _idSystem.Iterate([this](const CellId id) {
        const bool isDead = _healthSystem.IsZero(id);
        if (isDead) {
            return;
        }
        _ageSystem.Increment(id, LimitCellAge);
    });

    // Cleanup world systems from dead cells.
    _graveyardSystem.Cleanup();

    // Spawn more if needed
    const uint32_t aliveCellsCount = _idSystem.GetCellsCount();
    if (aliveCellsCount == 0) {
        _selectionSystem.Restart();
        _spawnSystem.SetCellFactory(*_defaultCellFactory);
        _statistics.generation = 0;
        Respawn();
    } else {
        SelectionSystem::Result selectionResult = _selectionSystem.TickGeneration();
        if (selectionResult.shouldRespawn) {
            _spawnSystem.SetCellFactory(*selectionResult.cellFactory);
            _statistics.generation = selectionResult.generation;
            Respawn();
        }
    }

    _statistics.cellsCount = _idSystem.GetCellsCount();
    _statistics.tick += 1;
}

void WorldWhite::RegisterProcedures()
{
    _simulationVm.RegisterProcedure<MoveProcedure>(ProcedureType::Move, 1, 0, "move", _simulationVm, _positionSystem);
    _simulationVm.RegisterProcedure<ConsumeProcedure>(ProcedureType::Consume, 1, 0, "consume", _simulationVm, _positionSystem, _healthSystem, _typeSystem);
    _simulationVm.RegisterProcedure<LookProcedure>(ProcedureType::Look, 1, 1, "look", _simulationVm, _positionSystem, _typeSystem);
    _simulationVm.RegisterProcedure<ReproductionProcedure>(ProcedureType::Reproduction, 1, 0, "reproduction", _simulationVm, _positionSystem, _healthSystem, _brainSystem, _typeSystem, _spawner);
}

WorldRender::Config WorldWhite::MakeRenderConfig(uint32_t cellSize, std::unique_ptr<sf::Shader> shader)
{
    ASSERT(cellSize < 255);
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
        _spawner,
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
