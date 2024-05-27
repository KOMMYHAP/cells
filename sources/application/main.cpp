#include "registrar/registrar.h"

#include "common_system.h"
#include "main_loop.h"
#include "scripts_system.h"
#include "ui_system.h"
#include "world.h"

#include "native/lua_registrable_system.h"

int main(int argc, char** argv)
{
    common::Registrar registrar;
    registrar.Register(std::make_unique<common::CommonSystem>(argc, argv));
    registrar.Register(std::make_unique<ScriptSystem>());
    registrar.Register(std::make_unique<World>());
    registrar.Register(std::make_unique<UiSystem>());
    registrar.Register(std::make_unique<scripts::LuaSystem>());

    auto mainLoop = std::make_unique<MainLoop>();
    auto* rawMainLoop = mainLoop.get();
    registrar.Register(std::move(mainLoop));

    const auto error = registrar.RunInit();
    if (error) {
        std::cerr << std::format("Failed to initialize systems: {}", error.message()) << std::endl;
        return -1;
    }

    rawMainLoop->Run();

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
