#include "command_line.h"
#include "registrar/registrar.h"

#include "main_loop.h"
#include "simulation_registrable_system.h"
#include "ui_system.h"

class StubCmdLine : public common::RegistrableSystem {
public:
    StubCmdLine(int argc, char** argv)
        : argc(argc)
        , argv(argv)
    {
    }

    std::error_code InitializeSystem(ApplicationStorage& storage) override
    {
        storage.Store<common::CommandLine>(argc, argv);
        return {};
    }
    void TerminateSystem() override { }

private:
    int argc;
    char** argv;
};

int main(int argc, char** argv)
{
    common::Registrar registrar;
    registrar.Register(std::make_unique<StubCmdLine>(argc, argv));
    registrar.Register(std::make_unique<SimulationRegistrableSystem>());
    registrar.Register(std::make_unique<UiSystem>());
    auto& mainLoop = registrar.Register(std::make_unique<MainLoop>());

    if (const std::error_code error = registrar.RunInit()) {
        std::cerr << std::format("Failed to initialize systems: {}", error.message()) << std::endl;
        return -1;
    }

    mainLoop.Run();

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
