#include "world_white.h"

#include <SFML/Graphics.hpp>

#include "procedures/move_procedure.h"
#include "random.h"

WorldWhite::WorldWhite(Config&& config)
    : _idSystem(config.width * config.height)
    , _brainSystem(_idSystem.GetCellsCountLimit())
    , _typeSystem(_idSystem.GetCellsCountLimit())
    , _positionSystem(config.width, config.height)
    , _cellFactory(_simulationVm, _brainSystem)
    , _graveyardSystem(_idSystem.GetCellsCountLimit(), _idSystem, _typeSystem, _positionSystem)
    , _healthSystem(_idSystem.GetCellsCountLimit(), _graveyardSystem)
    , _simulationVm(MakeSimulationVmConfig(this))
    , _render(MakeRenderConfig(config.cellSize, std::move(config.shader)), _positionSystem, _idSystem, _typeSystem)
{
    RegisterProcedures();
    MakeTestField(config.fullnessPercent);
}

void WorldWhite::Tick()
{
    // Process brain of each cell.
    _idSystem.Iterate([this](const CellId id) {
        _simulationVm.Run(id);
    });

    // Cleanup world systems from dead cells.
    _graveyardSystem.Cleanup();
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

void WorldWhite::MakeTestField(uint8_t fullnessPercent)
{
    const uint16_t moveCommandsCount = std::min<uint16_t>(_positionSystem.GetWidth(), 3);

    std::vector<sf::Vector2<int16_t>> positions;
    positions.reserve(_idSystem.GetCellsCountLimit());
    for (uint16_t x { 0 }; x < _positionSystem.GetWidth(); ++x) {
        for (uint16_t y { 0 }; y < _positionSystem.GetHeight(); ++y) {
            positions.emplace_back(x, y);
        }
    }
    std::shuffle(positions.begin(), positions.end(), common::GetRandomEngine());

    const auto countLimit = static_cast<uint32_t>(std::round(positions.size() * (static_cast<float>(fullnessPercent) / 100)));
    //    const auto countLimit = 1;

    for (const auto& position : std::span(positions).first(countLimit)) {
        const CellId id = _idSystem.Create();
        _positionSystem.Set(id, position);
        _typeSystem.Set(id, CellType::Unit);
        _cellFactory.MakePatrolUnit(id, moveCommandsCount);
    }
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
