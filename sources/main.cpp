#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <windows.h>

#include <processthreadsapi.h>

#include "basic_defines.h"

#include "breakpoint.h"
#include "command_line.h"
#include "profile/profile.h"
#include "simulation.h"
#include "world.h"
#include "world_render.h"

const std::string_view FontArgument = "--font";
const std::string_view FragmentShaderArgument = "--fragment-shader";

const float TargetTicksPerSeconds = 1.f;
const sf::Time TargetSimulationTime = sf::seconds(1.0f / TargetTicksPerSeconds);
const uint8_t CellsCountPercentOfLimit = 20;

const uint16_t ScreenWidth = 800;
const uint16_t ScreenHeight = 600;

const uint16_t FieldOffset = 20;
const uint16_t FieldWidth = ScreenWidth - 2 * FieldOffset;
const uint16_t FieldHeight = ScreenHeight - 2 * FieldOffset;

const uint16_t StatusTextOffset = 5;
const uint16_t StatusTextSize = 10;

const uint16_t CellPadding = 0;
const uint16_t CellSize = 8;

const uint16_t RowsCount = FieldHeight / (CellSize + CellPadding);
const uint16_t ColumnsCount = FieldWidth / (CellSize + CellPadding);

const sf::Color Gray { 0xCCCCCCFF };
const uint16_t StatusMessageBufferLimit = 200;

void MakeTestField(World& world, uint8_t percent)
{
    std::default_random_engine randomEngine;
    const uint16_t moveCommandsCount = std::min<uint16_t>(world.positionSystem.GetWidth(), 3);

    std::vector<sf::Vector2<int16_t>> positions;
    positions.reserve(world.idSystem.GetCellsCountLimit());
    for (uint16_t x { 0 }; x < world.positionSystem.GetWidth(); ++x) {
        for (uint16_t y { 0 }; y < world.positionSystem.GetHeight(); ++y) {
            positions.emplace_back(x, y);
        }
    }
    std::shuffle(positions.begin(), positions.end(), randomEngine);

    const auto countLimit = static_cast<uint32_t>(std::round(positions.size() * (static_cast<float>(percent) / 100)));
//    const auto countLimit = 1;

    for (const auto& position : std::span(positions).first(countLimit)) {
        const CellId id = world.idSystem.Create();
        world.positionSystem.Set(id, position);
        world.typeSystem.Set(id, CellType::Unit);
        world.cellFactory.MakePatrolUnit(id, moveCommandsCount);
    }
}

auto GetTimeInfo(sf::Time time)
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
    common::EnableBreakpointOnAssert(true);

    assert(StatusTextOffset * 2 + StatusTextSize <= FieldOffset);
    assert(FieldWidth % (CellSize + CellPadding) == 0);
    assert(FieldHeight % (CellSize + CellPadding) == 0);

    common::CommandLine commandLine { argc, argv };

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
    window.setFramerateLimit(60);

    World world { ColumnsCount, RowsCount };

    Simulation simulation { world };
    simulation.SetAutoMode(TargetTicksPerSeconds, TargetSimulationTime);

    WorldRender::Config renderConfig {
        std::move(shader),
        { sf::Color::Magenta, sf::Color::Green, sf::Color::Black, Gray },
        CellSize
    };
    WorldRender render { world, std::move(renderConfig) };

    MakeTestField(world, CellsCountPercentOfLimit);

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

    sf::Time frameElapsedTime = sf::milliseconds(15);

    common::SampleCounter<float, 99> frameTimeCounter;
    common::SampleCounter<uint32_t, 99> ticksCounter;

    while (window.isOpen()) {
        common::ProfileScope frameProfileScope { "Frame", mainCategory };

        sf::Event event {};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        const uint32_t elapsedTicks = simulation.Run(frameElapsedTime);

        {
            frameTimeCounter.AddSample(frameElapsedTime.asSeconds());
            sf::Time frameTime = sf::seconds(frameTimeCounter.CalcMedian());
            const auto [frameTimeValue, frameUnit] = GetTimeInfo(frameTime);
            const auto fps = static_cast<uint16_t>(1.0f / frameTime.asSeconds());

            ticksCounter.AddSample(fps * elapsedTicks);

            const uint32_t cellsCount = world.idSystem.GetCellsCount();
            const uint8_t cellsCountPercent = static_cast<uint8_t>(static_cast<float>(cellsCount) / (RowsCount * ColumnsCount) * 100.0f);

            statusMessageBuffer.clear();
            std::format_to_n(std::back_inserter(statusMessageBuffer), statusMessageBuffer.capacity(),
                "FPS {:4} | Frame {:4}{:2} | Ticks/sec {:3} | Cells {:8} ({:2}%)",
                fps,
                frameTimeValue, frameUnit,
                ticksCounter.CalcAverage(),
                cellsCount, cellsCountPercent);
            statusText.setString(sf::String(statusMessageBuffer));
        }

        window.clear(Gray);
        render.Render(window, rootStates);
        window.draw(statusText);
        window.display();

        frameElapsedTime = frameClock.getElapsedTime();
        frameClock.restart();
    }

    return 0;
}
