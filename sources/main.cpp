#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <windows.h>

#include <processthreadsapi.h>

#include "basicdefs.h"

#include "brain/brain.h"
#include "brain/brain_processor.h"
#include "command_line.h"
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

Cell CreatePatrolUnit(uint8_t offset, const CellPosition& position, const uint16_t moveCommandsCount)
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
        const int16_t patrolCellInitialPos = i + 1;
        const Cell& cell = CreatePatrolUnit(moveCommandOffset, { patrolCellInitialPos, patrolCellInitialPos }, moveCommandsCount);
        field.Create(cell);
    }
}

void MakeTestFieldV2(Field& field, uint8_t percent)
{
    std::default_random_engine randomEngine;
    const uint16_t moveCommandsCount = std::min<uint16_t>(field.GetColumnsCount() - 3, 50);
    std::uniform_int_distribution<uint16_t> uniformDist(0, moveCommandsCount - 1);

    std::vector<sf::Vector2<int16_t>> positions;
    positions.reserve(field.GetColumnsCount() * field.GetRowsCount());
    for (int16_t x { 0 }; x < field.GetColumnsCount(); ++x) {
        for (int16_t y { 0 }; y < field.GetRowsCount(); ++y) {
            positions.emplace_back(x, y);
        }
    }
    std::shuffle(positions.begin(), positions.end(), randomEngine);

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

std::optional<std::filesystem::path> ParseSandboxDirectory(int argc, char** argv)
{
    std::string_view helpMessage = "Please specify path to sandbox using --sandbox $directory";

    if (argc != 3) {
        std::cerr << helpMessage << std::endl;
        return {};
    }
    if (std::string_view(argv[1]) != "--sandbox") {
        std::cerr << helpMessage << std::endl;
        return {};
    }
    std::error_code ec;
    std::filesystem::path sandbox { argv[2] };
    if (!is_directory(sandbox, ec)) {
        std::cerr << std::format("Sandbox must be a directory: error code {}", ec.message()) << std::endl;
        return {};
    }

    return std::move(sandbox);
}

int main(int argc, char** argv)
{
    const std::string_view FontArgument = "--font";
    const std::string_view FragmentShaderArgument = "--fragment-shader";

    const sf::Time TargetSimulationTime = sf::milliseconds(15);
    const float SimulationTimeScalingUpCoef = 0.5f;
    const float SimulationTimeScalingDownCoef = 1.0f;
    const uint8_t CellsCountPercentOfLimit = 80;

    const uint16_t ScreenWidth = 800;
    const uint16_t ScreenHeight = 600;

    const uint16_t FieldOffset = 20;
    const uint16_t FieldWidth = ScreenWidth - 2 * FieldOffset;
    const uint16_t FieldHeight = ScreenHeight - 2 * FieldOffset;

    const uint16_t StatusTextOffset = 5;
    const uint16_t StatusTextSize = 10;
    assert(StatusTextOffset * 2 + StatusTextSize <= FieldOffset);

    const uint16_t CellPadding = 0;
    const uint16_t CellSize = 8;

    assert(FieldWidth % (CellSize + CellPadding) == 0);
    assert(FieldHeight % (CellSize + CellPadding) == 0);

    const uint16_t RowsCount = FieldHeight / (CellSize + CellPadding);
    const uint16_t ColumnsCount = FieldWidth / (CellSize + CellPadding);

    const sf::Color Gray { 0xCCCCCCFF };
    const uint16_t StatusMessageBufferLimit = 200;

    common::CommandLine commandLine { argc, argv };

    auto previous_handler = std::signal(SIGABRT, signalHandler);
    if (previous_handler == SIG_ERR) {
        std::cerr << "Failed to set signal handler for SIGABRT" << std::endl;
        return -1;
    }

    HRESULT hr = SetThreadDescription(GetCurrentThread(), L"main");
    if (FAILED(hr)) {
        std::cerr << "Failed to set name for main thread" << std::endl;
        return -1;
    }

    auto mbFontPath = commandLine.TryFindValue(FontArgument);
    if (!mbFontPath.has_value()) {
        std::cerr << std::format("Please specify filepath to font file using {} $path", FontArgument) << std::endl;
        return -1;
    }

    sf::Font defaultFont;
    if (!defaultFont.loadFromFile(std::string { *mbFontPath })) {
        return -1;
    }

    auto mbFragmentShaderPath = commandLine.TryFindValue(FragmentShaderArgument);
    if (!mbFragmentShaderPath.has_value()) {
        std::cerr << std::format("Please specify filepath to fragment shader using {} $path", FragmentShaderArgument) << std::endl;
        return -1;
    }

    auto shader = std::make_unique<sf::Shader>();
    if (!shader->loadFromFile(std::string { *mbFragmentShaderPath }, sf::Shader::Fragment)) {
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Cells", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(false);

    Field field { RowsCount, ColumnsCount };
    Simulation simulation { field };

    WorldRender::Config renderConfig {
        std::move(shader),
        { sf::Color::Magenta, sf::Color::Green, sf::Color::Black, Gray },
        CellSize
    };
    WorldRender render { field, std::move(renderConfig) };

    MakeTestFieldV2(field, CellsCountPercentOfLimit);

    sf::Clock frameClock;
    sf::Clock simulationClock;

    sf::Text statusText;
    statusText.setFont(defaultFont);
    statusText.setCharacterSize(StatusTextSize);
    statusText.setPosition(FieldOffset + StatusTextOffset, StatusTextOffset);

    const auto mainCategory = common::MakeProfileCategory();
    std::string statusMessageBuffer;
    statusMessageBuffer.reserve(StatusMessageBufferLimit);

    sf::RenderStates rootStates;
    rootStates.transform.translate(FieldOffset, FieldOffset);

    sf::Time frameElapsedTime;

    float simulationTicks = 0.0f;
    simulation.AddTicksToUpdate(simulationTicks);

    while (window.isOpen()) {
        common::ProfileScope frameProfileScope { "Frame", mainCategory };

        sf::Event event {};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        simulationClock.restart();
        simulation.Update(frameElapsedTime);
        const sf::Time simulationTime = simulationClock.getElapsedTime();

        const float downgradeCoef = simulationTime.asSeconds() / TargetSimulationTime.asSeconds();
        if (downgradeCoef >= 1.0) {
            simulationTicks -= SimulationTimeScalingDownCoef * downgradeCoef;
        } else {
            simulationTicks += SimulationTimeScalingUpCoef * (1.0f - downgradeCoef);
        }
        simulationTicks = std::max(simulationTicks, 0.0f);
        simulation.AddTicksToUpdate(simulationTicks);

        const auto [frameTimeValue, frameUnit] = GatherTimeInfo(frameElapsedTime);

        const float fps = frameElapsedTime != sf::Time::Zero ? 1 / frameElapsedTime.asSeconds() : 0.0f;
        const uint32_t cellsCount = field.GetCellsCount();
        const uint8_t cellsCountPercent = static_cast<uint8_t>(static_cast<float>(field.GetCellsCount()) / (field.GetColumnsCount() * field.GetRowsCount()) * 100.0f);

        statusMessageBuffer.clear();
        std::format_to_n(std::back_inserter(statusMessageBuffer), statusMessageBuffer.capacity(),
            "FPS {:6.2f} | Frame {:4}{:2} | Ticks {:4} | Cells {:8} ({:2}%)",
            fps,
            frameTimeValue, frameUnit,
            simulation.GetUpdateStatistics().processedTicks,
            cellsCount, cellsCountPercent);
        statusText.setString(sf::String(statusMessageBuffer));

        window.clear(Gray);
        render.Render(window, rootStates);
        window.draw(statusText);
        window.display();

        frameElapsedTime = frameClock.getElapsedTime();
        frameClock.restart();
    }

    return 0;
}
