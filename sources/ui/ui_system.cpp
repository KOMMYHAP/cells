#include "ui_system.h"

#include "command_line.h"
#include "storage/stack_storage.h"

#include "ui_layout.h"
#include "world.h"
#include "world_render.h"

static const std::string_view FontArgument = "--font";

std::error_code UiSystem::InitializeSystem(common::StackStorage& storage)
{
    const auto& layout = storage.Get<UiLayout>();
    _window.create(sf::VideoMode(layout.screenWidth, layout.screenHeight), "Cells", sf::Style::Titlebar | sf::Style::Close);
    ASSERT(_window.isOpen());

    _window.setVerticalSyncEnabled(false);
    _window.setFramerateLimit(60);

    auto* world = storage.Get<World*>();
    _worldRender = &world->GetSystems().Modify<WorldRender>();
    _worldStates.transform.translate(static_cast<float>(layout.fieldOffset), static_cast<float>(layout.fieldOffset));

    auto mbFontPath = storage.Get<common::CommandLine>().FindValue(FontArgument);
    ASSERT(mbFontPath.has_value(), "you should specify font path via --font argument!");

    _font = std::make_unique<sf::Font>();
    const bool fontLoaded = _font->loadFromFile(std::string { *mbFontPath });
    ASSERT(fontLoaded, "invalid font!");

    _statusPanel = std::make_unique<StatusPanel>(layout, *_font, *world);

    storage.Store<UiSystem*>(this);
    return std::error_code();
}

void UiSystem::TerminateSystem()
{
    _worldRender = nullptr;
    _window.close();
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

void UiSystem::Update(sf::Time elapsedTime)
{
    _statusPanel->Update(elapsedTime);
}

void UiSystem::Render()
{
    if (!_window.isOpen()) {
        return;
    }

    const sf::Color gray { 0xCCCCCCFF };
    _window.clear(gray);

    _statusPanel->Draw(_window);
    _worldRender->Render(_window, _worldStates);

    _window.display();
}
