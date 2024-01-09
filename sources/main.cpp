#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "brain/brain.h"
#include "field/field.h"
#include "processor/brain_processor.h"
#include "simulation.h"
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
    Cell cell = BrainProcessor::MakeDefaultUnit();
    Brain brain { cell };
    brain.AccessInfo().position = position;

    Memory dataScope = brain.Access();
    BrainControlBlock& controlBlock = dataScope.Get<BrainControlBlock>();

    constexpr uint8_t moveCommandSize = static_cast<uint8_t>(sizeof(CommandParam) + sizeof(Direction));
    controlBlock.nextCommand = offset * moveCommandSize;

    for (int i = 0; i < moveCommandsCount; ++i) {
        dataScope.Write(UnitCommand::Move, Direction::Right);
    }
    for (int i = 0; i < moveCommandsCount; ++i) {
        dataScope.Write(UnitCommand::Move, Direction::Left);
    }
    dataScope.Write(SystemCommand::Jump, CommandParam { 0 });

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
        Brain brain { cell };
        CellInfo& info = brain.AccessInfo();
        info.position = sf::Vector2<uint16_t>(x, 0);
        info.type = CellType::Wall;
        field.Create(cell);

        info.position = sf::Vector2<uint16_t>(x, rowsCount - 1);
        field.Create(cell);
    }
    for (int y = 0; y < rowsCount; ++y) {
        Cell cell;
        Brain brain { cell };
        CellInfo& info = brain.AccessInfo();
        info.position = sf::Vector2<uint16_t>(0, y);
        info.type = CellType::Wall;
        field.Create(cell);

        info.position = sf::Vector2<uint16_t>(columnsCount - 1, y);
        field.Create(cell);
    }

    const uint16_t moveCommandsCount = std::min(columnsCount - 3, 50);
    const uint16_t patrolCellToCreate = rowsCount - 2;
    for (int i = 0; i < patrolCellToCreate; ++i) {
        const uint8_t moveCommandOffset = i % moveCommandsCount;
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
