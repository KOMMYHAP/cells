#include "breakpoint.h"
#include "command_line.h"
#include "common_system.h"
#include "main_window.h"
#include "profile/profile.h"
#include "random.h"
#include "scripts_system.h"
#include "simulation.h"
#include "ui_layout.h"
#include "world.h"
#include "world_render.h"
#include "world_widget.h"
#include <registrar/registrar.h>

#include "storage/storage.h"

#include "setup_script.h"
#include "simulation_script.h"

#include "systems/age_system.h"
#include "systems/brain_system.h"
#include "systems/graveyard_system.h"
#include "systems/health_system.h"
#include "systems/id_system.h"
#include "systems/position_system.h"
#include "systems/selection_system.h"
#include "systems/simulation_virtual_machine.h"
#include "systems/spawn_system.h"
#include "systems/spawner.h"
#include "systems/type_system.h"

const std::string_view FontArgument = "--font";
const std::string_view FragmentShaderArgument = "--fragment-shader";

const sf::Time TargetSimulationTime = sf::milliseconds(15);
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

int main(int argc, char** argv)
{
    common::Registrar registrar;
    registrar.Register(std::make_unique<common::CommonSystem>(argc, argv));
    registrar.Register(std::make_unique<ScriptSystem>());
    registrar.Register(std::make_unique<World>());
    registrar.Register(std::make_unique<UiSystem>());

    const auto result = registrar.RunInit();
    if (!result) {
        std::cerr << std::format("Failed to initialize systems: {}") << std::endl;
        return -1;
    }

    return 0;
    //
    //    sf::Clock frameClock;
    //    sf::Clock simulationClock;
    //
    //    sf::Text statusText;
    //    statusText.setFont(defaultFont);
    //    statusText.setCharacterSize(StatusTextSize);
    //    statusText.setPosition(FieldOffset + StatusTextOffset, StatusTextOffset);
    //
    //    const auto mainCategory = common::MakeProfileCategory();
    //    std::string statusMessageBuffer;
    //    statusMessageBuffer.reserve(StatusMessageBufferLimit);
    //
    //    sf::Time frameElapsedTime;
    //
    //    common::SampleCounter<float, 10> frameTimeCounter;
    //
    //    while (window.isOpen()) {
    //        common::ProfileScope frameProfileScope { "Frame", mainCategory };
    //
    //        sf::Event event {};
    //        while (window.pollEvent(event)) {
    //            if (event.type == sf::Event::Closed) {
    //                window.close();
    //            }
    //        }
    //
    //        simulation.Run(frameElapsedTime);
    //
    //        {
    //            frameTimeCounter.AddSample(frameElapsedTime.asSeconds());
    //            sf::Time frameTime = sf::seconds(frameTimeCounter.CalcMedian());
    //            const auto [frameTimeValue, frameUnit] = GetTimeInfo(frameTime);
    //            const auto fps = static_cast<uint16_t>(1.0f / frameTime.asSeconds());
    //
    //            const World::Statistics statistics = world.GetStatistics();
    //            const uint8_t cellsCountPercent = static_cast<uint8_t>(static_cast<float>(statistics.cellsCount) / (RowsCount * ColumnsCount) * 100.0f);
    //
    //            statusMessageBuffer.clear();
    //            std::format_to_n(std::back_inserter(statusMessageBuffer), statusMessageBuffer.capacity(),
    //                "FPS {:4} | Frame {:4}{:2} | Cells {:8} ({:2}%) | Generation #{:3} | Tick #{:9}",
    //                fps,
    //                frameTimeValue, frameUnit,
    //                statistics.cellsCount, cellsCountPercent,
    //                statistics.generation,
    //                statistics.tick);
    //            statusText.setString(sf::String(statusMessageBuffer));
    //        }
    //
    //        window.clear(Gray);
    //        world.Render(window, rootStates);
    //        window.draw(statusText);
    //        window.display();
    //
    //        frameElapsedTime = frameClock.getElapsedTime();
    //        frameClock.restart();
    //    }
    //
    //
    //    return 0;
}
