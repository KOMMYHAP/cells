#include "ui_system.h"

#include "storage/stack_storage.h"

#include "ui_layout.h"
#include "world.h"
#include "world_render.h"

std::error_code UiSystem::InitializeSystem(common::StackStorage& storage)
{
    const auto& layout = storage.Get<UiLayout>();
    _window.create(sf::VideoMode(layout.screenWidth, layout.screenHeight), "Cells", sf::Style::Titlebar | sf::Style::Close);
    ASSERT(_window.isOpen());

    _window.setVerticalSyncEnabled(false);
    _window.setFramerateLimit(60);

    _worldRender = storage.Get<World*>()->GetSystems().Modify<WorldRender>();
    _worldStates.transform.translate(static_cast<float>(layout.fieldOffset), static_cast<float>(layout.fieldOffset));

    storage.Store<UiSystem*>(this);
    return std::error_code();
}

void UiSystem::TerminateSystem()
{
    _worldWidget.reset();
    _window.close();
}

void UiSystem::Render()
{
    if (!_window.isOpen()) {
        return;
    }

    const sf::Color gray { 0xCCCCCCFF };
    _window.clear(gray);

    _worldRender.Render(_window, _worldStates);

    _window.display();
}

UiSystem::MainLoopFeedback UiSystem::ProcessInput()
{
    auto feedback { MainLoopFeedback::ShouldRun };
    sf::Event event {};
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            feedback = MainLoopFeedback::ShouldStop;
        }
    }
    return feedback;
}
