#include <csignal>
#include <cstdint>
#include <limits>
#include <span>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "brain.h"
#include "field.h"
#include "simulation.h"
#include "unit_processor.h"
#include "world_render.h"

void signalHandler(int signal)
{
    if (signal == SIGABRT) {
        int breakOnMe;
    }
    std::_Exit(EXIT_FAILURE);
}

Cell CreatePatrolUnit(uint8_t offset, const sf::Vector2<uint16_t>& position, const uint16_t moveCommandsCount)
{
    Cell cell = UnitProcessor::MakeDefaultUnit();
    Brain brain = cell.GetBrain();
    brain.AccessInfo().position = position;

    BrainData dataScope = brain.AccessData();
    UnitControlBlock& controlBlock = dataScope.Pop<UnitControlBlock>();

    constexpr uint8_t moveCommandSize = static_cast<uint8_t>(sizeof(UnitCommandParam) + sizeof(Direction));
    controlBlock.nextCommand = offset * moveCommandSize;

    for (int i = 0; i < moveCommandsCount; ++i) {
        UnitCommandParam& move = dataScope.Pop<UnitCommandParam>();
        move.value = static_cast<std::underlying_type_t<UnitCommand>>(UnitCommand::Move);
        Direction& direction = dataScope.Pop<Direction>();
        direction = Direction::Right;
    }
    for (int i = 0; i < moveCommandsCount; ++i) {
        UnitCommandParam& move = dataScope.Pop<UnitCommandParam>();
        move.value = static_cast<std::underlying_type_t<UnitCommand>>(UnitCommand::Move);
        Direction& direction = dataScope.Pop<Direction>();
        direction = Direction::Left;
    }
    {
        UnitCommandParam& jump = dataScope.Pop<UnitCommandParam>();
        jump.value = static_cast<std::underlying_type_t<UnitCommand>>(UnitCommand::Jump);
        UnitCommandParam& destination = dataScope.Pop<UnitCommandParam>();
        destination.value = 0;
    }

    return cell;
}

int main()
{
    auto previous_handler = std::signal(SIGABRT, signalHandler);
    if (previous_handler == SIG_ERR) {
        return EXIT_FAILURE;
    }

    const uint16_t screenWidth = 800;
    const uint16_t screenHeight = 600;

    const uint16_t fieldOffset = 20;
    const uint16_t fieldWidth = screenWidth - 2 * fieldOffset;
    const uint16_t fieldHeight = screenHeight - 2 * fieldOffset;

    const uint16_t rowsCount = 140;
    const uint16_t columnsCount = 190;

    assert(fieldWidth % columnsCount == 0);
    assert(fieldHeight % rowsCount == 0);

    const uint16_t cellPadding = 0;
    const uint16_t cellSizeX = fieldWidth / (columnsCount + cellPadding);
    const uint16_t cellSizeY = fieldHeight / (rowsCount + cellPadding);

    assert(cellSizeX == cellSizeY);
    assert(cellSizeX > 0);
    const uint16_t cellSize = cellSizeX;

    Field field { rowsCount, columnsCount };
    Simulation simulation { field };

    CellRender::Config cellRenderConfig {
        static_cast<float>(cellPadding), static_cast<float>(cellPadding),
        cellSize,
        { static_cast<float>(fieldOffset), static_cast<float>(fieldOffset) },
        { sf::Color::Magenta, sf::Color::Green, sf::Color::Black }
    };
    WorldRender render { field, std::move(cellRenderConfig) };

    for (int x = 0; x < columnsCount; ++x) {
        Cell cell;
        Brain brain = cell.GetBrain();
        BrainInfo& info = brain.AccessInfo();
        info.position = sf::Vector2<uint16_t>(x, 0);
        info.type = CellType::Wall;
        field.Create(cell);

        info.position = sf::Vector2<uint16_t>(x, rowsCount - 1);
        field.Create(cell);
    }
    for (int y = 0; y < rowsCount; ++y) {
        Cell cell;
        Brain brain = cell.GetBrain();
        BrainInfo& info = brain.AccessInfo();
        info.position = sf::Vector2<uint16_t>(0, y);
        info.type = CellType::Wall;
        field.Create(cell);

        info.position = sf::Vector2<uint16_t>(columnsCount - 1, y);
        field.Create(cell);
    }

    const uint16_t moveCommandsCount = columnsCount - 3;
    const uint16_t patrolCellToCreate = rowsCount - 2;
    for (int i = 0; i < patrolCellToCreate; ++i) {
        const uint8_t moveCommandOffset = i;
        const uint16_t patrolCellInitialPos = i + 1;
        const Cell& cell = CreatePatrolUnit(moveCommandOffset, { patrolCellInitialPos, patrolCellInitialPos }, moveCommandsCount);
        field.Create(cell);
    }

    simulation.SetAutoUpdateMode(100);

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Cells");
    window.setFramerateLimit(60);

    sf::Clock clock;
    const sf::Color gray { 0xCCCCCCFF };

    while (window.isOpen()) {
        const sf::Time elapsedTime = clock.getElapsedTime();
        clock.restart();

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event {};
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        simulation.Update(elapsedTime);

        window.clear(gray);
        render.Render(window);
        window.display();
    }
    return 0;
}
