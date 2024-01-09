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

    Memory memory = brain.AccessMemory();
    BrainControlBlock& controlBlock = memory.Get<BrainControlBlock>();

    constexpr uint8_t moveCommandSize = static_cast<uint8_t>(sizeof(CommandParam) + sizeof(Direction));
    controlBlock.nextCommand = offset * moveCommandSize;

    for (int i = 0; i < moveCommandsCount; ++i) {
        memory.Write(UnitCommand::Move, Direction::Right);
    }
    for (int i = 0; i < moveCommandsCount; ++i) {
        memory.Write(UnitCommand::Move, Direction::Left);
    }
    memory.Write(SystemCommand::Jump, CommandParam { 0 });

    return cell;
}

void MakeTestFieldV1(Field& field)
{
    const uint16_t moveCommandsCount = std::min<uint16_t>(field.GetColumnsCount() - 3, 50);
    const uint16_t patrolCellToCreate = field.GetRowsCount() - 2;
    for (int i = 0; i < patrolCellToCreate; ++i) {
        const uint8_t moveCommandOffset = i % moveCommandsCount;
        const uint16_t patrolCellInitialPos = i + 1;
        const Cell& cell = CreatePatrolUnit(moveCommandOffset, { patrolCellInitialPos, patrolCellInitialPos }, moveCommandsCount);
        field.Create(cell);
    }
}

void MakeTestFieldV2(Field& field)
{
    std::default_random_engine randomEngine;
    const uint16_t moveCommandsCount = std::min<uint16_t>(field.GetColumnsCount() - 3, 50);
    std::uniform_int_distribution<uint8_t> uniformDist(0, moveCommandsCount - 1);

    std::vector<sf::Vector2<uint16_t>> positions;
    positions.reserve(field.GetColumnsCount() * field.GetRowsCount());
    for (uint16_t x { 1 }; x < field.GetColumnsCount() - 1; ++x) {
        for (uint16_t y { 1 }; y < field.GetRowsCount() - 1; ++y) {
            positions.emplace_back(x, y);
        }
    }
    std::shuffle(positions.begin(), positions.end(), randomEngine);

    const uint16_t countLimit = std::min<uint16_t>(1000, positions.size());

    for (const auto& position : std::span(positions).first(countLimit)) {
        const uint8_t moveCommandOffset = uniformDist(randomEngine);
        const Cell& cell = CreatePatrolUnit(moveCommandOffset, position, moveCommandsCount);
        field.Create(cell);
    }
}

int main(int argc, char** argv)
{
    auto previous_handler = std::signal(SIGABRT, signalHandler);
    if (previous_handler == SIG_ERR) {
        return EXIT_FAILURE;
    }

    if (argc != 2) {
        std::cerr << "Please specify path to font" << std::endl;
        return 1;
    }

    sf::Font defaultFont;
    if (!defaultFont.loadFromFile(argv[1])) {
        return 1;
    }

    const uint16_t screenWidth = 800;
    const uint16_t screenHeight = 600;

    const uint16_t fieldOffset = 20;
    const uint16_t fieldWidth = screenWidth - 2 * fieldOffset;
    const uint16_t fieldHeight = screenHeight - 2 * fieldOffset;

    const uint16_t statusTextOffset = 5;
    const uint16_t statusTextSize = 10;
    assert(statusTextOffset * 2 + statusTextSize <= fieldOffset);

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

    //        MakeTestFieldV1(field);
    MakeTestFieldV2(field);

    simulation.SetAutoUpdateMode(100);

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Cells");
    window.setVerticalSyncEnabled(false);
    //    window.setFramerateLimit(60);

    sf::Clock frameClock;
    const sf::Color gray { 0xCCCCCCFF };

    sf::Text statusText;
    statusText.setFont(defaultFont);
    statusText.setCharacterSize(statusTextSize);
    statusText.setPosition(fieldOffset + statusTextOffset, statusTextOffset);

    while (window.isOpen()) {
        const sf::Time elapsedTime = frameClock.getElapsedTime();
        frameClock.restart();

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event {};
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        simulation.Update(elapsedTime);

        const sf::Time tickTime = simulation.GetTickProcessingTime();
        const std::string_view tickUnit = tickTime.asMilliseconds() >= 1000
            ? "s"
            : tickTime.asMilliseconds() >= 1 ? "ms"
                                             : "us";
        const float tickTimeValue = tickTime.asMilliseconds() >= 1000
            ? tickTime.asSeconds()
            : tickTime.asMilliseconds() >= 1 ? static_cast<float>(tickTime.asMilliseconds())
                                             : static_cast<float>(tickTime.asMicroseconds());

        const float fps = 1 / elapsedTime.asSeconds();

        std::string tickTimeText = std::format("FPS {:3.2f} | Tick {:2}{:2}", fps, tickTimeValue, tickUnit);
        statusText.setString(sf::String(tickTimeText));

        window.clear(gray);
        render.Render(window);
        window.draw(statusText);
        window.display();
    }
    return 0;
}
