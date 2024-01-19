#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <windows.h>

#include <processthreadsapi.h>

#include "basicdefs.h"

#include "brain/brain.h"
#include "brain/brain_processor.h"
#include "field/field.h"
#include "processor/memory.h"
#include "processor/processor_control_block.h"
#include "profile/profile.h"
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
    memory.Write(ProcessorInstruction::Jump, CommandParam { 0 });

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
    std::uniform_int_distribution<uint16_t> uniformDist(0, moveCommandsCount - 1);

    std::vector<sf::Vector2<uint16_t>> positions;
    positions.reserve(field.GetColumnsCount() * field.GetRowsCount());
    for (uint16_t x { 1 }; x < field.GetColumnsCount() - 1; ++x) {
        for (uint16_t y { 1 }; y < field.GetRowsCount() - 1; ++y) {
            positions.emplace_back(x, y);
        }
    }
    std::shuffle(positions.begin(), positions.end(), randomEngine);

    const uint8_t percent = 40;
    const auto countLimit = static_cast<uint32_t>(std::round(positions.size() * (static_cast<float>(percent) / 100)));

    for (const auto& position : std::span(positions).first(countLimit)) {
        const uint8_t moveCommandOffset = uniformDist(randomEngine);
        const Cell& cell = CreatePatrolUnit(moveCommandOffset, position, moveCommandsCount);
        field.Create(cell);
    }
}

auto GatherTimeInfo(sf::Time time)
{
    const std::string_view tickUnit = time.asMilliseconds() >= 1000 ? "s"
        : time.asMicroseconds() >= 1000                             ? "ms"
                                                                    : "us";
    const float tickTimeValue = time.asMilliseconds() >= 1000
        ? time.asSeconds()
        : time.asMicroseconds() >= 1000 ? static_cast<float>(time.asMilliseconds())
                                        : static_cast<float>(time.asMicroseconds());

    return std::make_tuple(tickTimeValue, tickUnit);
}

int main(int argc, char** argv)
{
    auto previous_handler = std::signal(SIGABRT, signalHandler);
    if (previous_handler == SIG_ERR) {
        return EXIT_FAILURE;
    }

    HRESULT hr = SetThreadDescription(GetCurrentThread(), L"main");
    if (FAILED(hr)) {
        // Call failed.
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

    const uint16_t cellPadding = 0;
    const uint16_t cellSize = 1;

    assert(fieldWidth % (cellSize + cellPadding) == 0);
    assert(fieldHeight % (cellSize + cellPadding) == 0);

    const uint16_t rowsCount = fieldHeight / (cellSize + cellPadding);
    const uint16_t columnsCount = fieldWidth / (cellSize + cellPadding);

    const uint32_t cellsPerPoint = 3; // food + cell + another cell
    Field field { rowsCount, columnsCount };
    Simulation simulation { field };

    CellRender::Config cellRenderConfig {
        static_cast<float>(cellPadding), static_cast<float>(cellPadding),
        cellSize,
        { static_cast<float>(fieldOffset), static_cast<float>(fieldOffset) },
        { sf::Color::Magenta, sf::Color::Green, sf::Color::Black, sf::Color::Transparent }
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
    for (int y = 1; y < rowsCount - 1; ++y) {
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

    const uint16_t simulationTicksPerSecond = 1;

    simulation.SetAutoUpdateMode(simulationTicksPerSecond);

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Cells");
    window.setVerticalSyncEnabled(false);
    //    window.setFramerateLimit(60);

    sf::Clock frameClock;
    sf::Clock simulationClock;
    const sf::Color gray { 0xCCCCCCFF };

    sf::Text statusText;
    statusText.setFont(defaultFont);
    statusText.setCharacterSize(statusTextSize);
    statusText.setPosition(fieldOffset + statusTextOffset, statusTextOffset);

    const auto mainCategory = common::MakeProfileCategory();
    std::string statusMessageBuffer;
    statusMessageBuffer.reserve(200);

    sf::RenderStates rootStates;
    rootStates.transform.translate(fieldOffset, fieldOffset);

    sf::Time frameElapsedTime;

    while (window.isOpen()) {
        common::ProfileScope frameProfileScope { "Frame", mainCategory };

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event {};
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        simulationClock.restart();
        simulation.Update(frameElapsedTime);
        const sf::Time simulationTime = simulationClock.getElapsedTime();
        const sf::Time tickTime = sf::seconds(simulationTime.asSeconds() / simulation.GetUpdateStatistics().processedTicks);
        const sf::Time processorTime = sf::seconds(tickTime.asSeconds() / field.GetCellsCount());

        const auto [frameTimeValue, frameUnit] = GatherTimeInfo(frameElapsedTime);
        const auto [simulationTimeValue, simulationTimeUnit] = GatherTimeInfo(simulationTime);
        const auto [tickTimeValue, tickUnit] = GatherTimeInfo(tickTime);
        const auto [processorTimeValue, processorUnit] = GatherTimeInfo(processorTime);

        const float fps = frameElapsedTime != sf::Time::Zero ? 1 / frameElapsedTime.asSeconds() : 0.0f;
        const uint32_t cellsCount = field.GetCellsCount();
        const uint8_t cellsCountPercent = static_cast<uint8_t>(static_cast<float>(field.GetCellsCount()) / (field.GetColumnsCount() * field.GetRowsCount()) * 100.0f);

        statusMessageBuffer.clear();
        std::format_to_n(std::back_inserter(statusMessageBuffer), statusMessageBuffer.capacity(),
            "FPS {:6.2f} | Frame {:4}{:2} | Simulation {:4}{:2} | Tick {:4}{:2} | Processor {:4}{:2} | Cells {:8} ({:2}%)",
            fps,
            frameTimeValue, frameUnit,
            simulationTimeValue, simulationTimeUnit,
            tickTimeValue, tickUnit,
            processorTimeValue, processorUnit,
            cellsCount, cellsCountPercent);
        statusText.setString(sf::String(statusMessageBuffer));

        window.clear(gray);
        render.Render(window, rootStates);
        window.draw(statusText);
        window.display();

        frameElapsedTime = frameClock.getElapsedTime();
        frameClock.restart();
    }

    return 0;
}
