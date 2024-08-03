#include "main_loop.h"
#include "world.h"
#include "ui_system.h"

std::error_code MainLoop::InitializeSystem(common::StackStorage& storage)
{
    _uiSystem = storage.Get<UiSystem*>();
    _world = &storage.Modify<World>();
    return {};
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