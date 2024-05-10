#include "main_loop.h"
#include "ui_system.h"
#include "world.h"

void MainLoop::Run()
{
    sf::Clock frameClock;
    while (!_stopRequested) {
        const auto feedback = _uiSystem->ProcessInput();
        if (feedback == UiSystem::MainLoopFeedback::ShouldStop) {
            _stopRequested = true;
        }

        _world->Update(frameClock.getElapsedTime());
        _uiSystem->Render();
        frameClock.restart();
    }
}
std::error_code MainLoop::InitializeSystem(common::StackStorage& storage)
{
    _uiSystem = storage.Get<UiSystem*>();
    _world = storage.Get<World*>();
    return std::error_code();
}

void MainLoop::TerminateSystem()
{
    _uiSystem = nullptr;
    _world = nullptr;
}
