#include <windows.h>

#include <processthreadsapi.h>

#include "basic_defines.h"

#include "breakpoint.h"
#include "command_line.h"
#include "profile/profile.h"
#include "random.h"
#include "simulation.h"
#include "world_white.h"

const std::string_view FontArgument = "--font";
const std::string_view FragmentShaderArgument = "--fragment-shader";

const float TargetTicksPerSeconds = 100.f;
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

struct CommonInitializationGuard {
    CommonInitializationGuard()
    {
        common::InitRandom("42");
    }
    ~CommonInitializationGuard()
    {
        common::TermRandom();
    }
};

int main(int argc, char** argv)
{
    CommonInitializationGuard commonInitializationGuard {};
    common::EnableBreakpointOnAssert(true);

    ASSERT(StatusTextOffset * 2 + StatusTextSize <= FieldOffset);
    ASSERT(FieldWidth % (CellSize + CellPadding) == 0);
    ASSERT(FieldHeight % (CellSize + CellPadding) == 0);

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
    window.setFramerateLimit(60);//

    WorldWhite::Config worldWhiteConfig {
        ColumnsCount, RowsCount, CellSize, CellsCountPercentOfLimit, std::move(shader), WorldWhite::SpawnPolicy::Random
    };
    WorldWhite world { std::move(worldWhiteConfig) };

    Simulation simulation { world };
    simulation.SetAutoMode(TargetTicksPerSeconds, TargetSimulationTime);

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

    while (window.isOpen()) {
        common::ProfileScope frameProfileScope { "Frame", mainCategory };

        sf::Event event {};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        simulation.Run(frameElapsedTime);

        {
            frameTimeCounter.AddSample(frameElapsedTime.asSeconds());
            sf::Time frameTime = sf::seconds(frameTimeCounter.CalcMedian());
            const auto [frameTimeValue, frameUnit] = GetTimeInfo(frameTime);
            const auto fps = static_cast<uint16_t>(1.0f / frameTime.asSeconds());

            const WorldWhite::Statistics statistics = world.GetStatistics();
            const uint8_t cellsCountPercent = static_cast<uint8_t>(static_cast<float>(statistics.cellsCount) / (RowsCount * ColumnsCount) * 100.0f);

            statusMessageBuffer.clear();
            std::format_to_n(std::back_inserter(statusMessageBuffer), statusMessageBuffer.capacity(),
                "FPS {:4} | Frame {:4}{:2} | Cells {:8} ({:2}%) | Generation #{:3} | Tick #{:9}",
                fps,
                frameTimeValue, frameUnit,
                statistics.cellsCount, cellsCountPercent,
                statistics.generation,
                statistics.tick);
            statusText.setString(sf::String(statusMessageBuffer));
        }

        window.clear(Gray);
        world.Render(window, rootStates);
        window.draw(statusText);
        window.display();

        frameElapsedTime = frameClock.getElapsedTime();
        frameClock.restart();
    }

    return 0;
}
