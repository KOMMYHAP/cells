#include "world_white.h"

#include <SFML/Graphics.hpp>

#include "procedures/move_procedure.h"

WorldWhite::WorldWhite(Config&& config)
    : idSystem(config.width * config.height)
    , brainSystem(idSystem.GetCellsCountLimit())
    , typeSystem(idSystem.GetCellsCountLimit())
    , positionSystem(config.width, config.height)
    , cellFactory(simulationVm, brainSystem)
    , graveyardSystem(idSystem.GetCellsCountLimit(), idSystem, typeSystem, positionSystem)
    , healthSystem(idSystem.GetCellsCountLimit(), graveyardSystem)
    , simulationVm(MakeSimulationVmConfig(this))
    , _render(MakeRenderConfig(config.cellSize, std::move(config.shader)), positionSystem, idSystem, typeSystem)
{
    RegisterProcedures();
    MakeTestField(config.fullnessPercent);
}

void WorldWhite::Tick()
{
    // Process brain of each cell.
    idSystem.Iterate([this](const CellId id) {
        simulationVm.Run(id);
    });

    // Cleanup world systems from dead cells.
    graveyardSystem.Cleanup();
}

void WorldWhite::RegisterProcedures()
{
    simulationVm.RegisterProcedure<MoveProcedure>(ProcedureType::Move, 1, 0, "move", simulationVm, positionSystem);
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
    std::default_random_engine randomEngine;
    const uint16_t moveCommandsCount = std::min<uint16_t>(positionSystem.GetWidth(), 3);

    std::vector<sf::Vector2<int16_t>> positions;
    positions.reserve(idSystem.GetCellsCountLimit());
    for (uint16_t x { 0 }; x < positionSystem.GetWidth(); ++x) {
        for (uint16_t y { 0 }; y < positionSystem.GetHeight(); ++y) {
            positions.emplace_back(x, y);
        }
    }
    std::shuffle(positions.begin(), positions.end(), randomEngine);

    const auto countLimit = static_cast<uint32_t>(std::round(positions.size() * (static_cast<float>(fullnessPercent) / 100)));
    //    const auto countLimit = 1;

    for (const auto& position : std::span(positions).first(countLimit)) {
        const CellId id = idSystem.Create();
        positionSystem.Set(id, position);
        typeSystem.Set(id, CellType::Unit);
        cellFactory.MakePatrolUnit(id, moveCommandsCount);
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
        const CellId id = world->simulationVm.GetRunningCellId();
        world->healthSystem.Set(id, CellHealth::Zero);
    };
    return {
        world->brainSystem,
        world->typeSystem,
        world->healthSystem,
        8,
        std::move(watcher)
    };
}
