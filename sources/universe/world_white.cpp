#include "world_white.h"

#include <SFML/Graphics.hpp>

#include "procedures/move_procedure.h"

WorldWhite::WorldWhite(Config&& config)
    : _idSystem(config.width * config.height)
    , _brainSystem(_idSystem.GetCellsCountLimit())
    , _typeSystem(_idSystem.GetCellsCountLimit())
    , _positionSystem(config.width, config.height)
    , _graveyardSystem(_idSystem.GetCellsCountLimit(), _idSystem, _typeSystem, _positionSystem)
    , _healthSystem(_idSystem.GetCellsCountLimit(), _graveyardSystem)
    , _simulationVm(MakeSimulationVmConfig(this))
    , _cellFactory(_simulationVm, _brainSystem)
    , _spawnSystem(MakeSpawnSystemConfig(config.fullnessPercent, SpawnSystem::Policy::RandomUnit))
    , _render(MakeRenderConfig(config.cellSize, std::move(config.shader)), _positionSystem, _idSystem, _typeSystem)
{
    RegisterProcedures();
}

void WorldWhite::Tick()
{
    // Process brain of each cell.
    _idSystem.Iterate([this](const CellId id) {
        _simulationVm.Run(id);
    });

    // Cleanup world systems from dead cells.
    _graveyardSystem.Cleanup();

    // Spawn more cells.
    _spawnSystem.Tick();
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

SpawnSystem::Config WorldWhite::MakeSpawnSystemConfig(float fullnessPercent, SpawnSystem::Policy policy)
{
    const auto targetPopulationSize = static_cast<uint32_t>(round(fullnessPercent * static_cast<float>(_idSystem.GetCellsCountLimit()) / 100.0f));
    return {
        _cellFactory,
        _positionSystem,
        _idSystem,
        _typeSystem,
        targetPopulationSize,
        policy
    };
}
