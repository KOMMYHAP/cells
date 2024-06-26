#include "main_loop.h"
#include "ui_system.h"
#include "world.h"

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

void MainLoop::Run()
{
    sf::Clock frameClock;
    while (!_stopRequested) {
        const sf::Time elapsedTime = frameClock.restart();

        const auto feedback = _uiSystem->ProcessInput();
        if (feedback == UiSystem::MainLoopFeedback::ShouldStop) {
            _stopRequested = true;
        }

        _world->Update(elapsedTime);
        _uiSystem->Update(elapsedTime);

        _uiSystem->Render();
    }
}