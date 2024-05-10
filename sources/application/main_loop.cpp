#include "main_loop.h"
#include "ui_system.h"
#include "world.h"

MainLoop::MainLoop(UiSystem& uiSystem, World& world)
    : _uiSystem(uiSystem)
    , _world(world)
{
}

void MainLoop::Run()
{
    sf::Clock frameClock;
    while (!_stopRequested) {
        const auto feedback = _uiSystem.ProcessInput();
        if (feedback == UiSystem::MainLoopFeedback::ShouldStop) {
            _stopRequested = true;
        }

        _world.Update(frameClock.getElapsedTime());
        _uiSystem.Render();
        frameClock.restart();
    }
}
