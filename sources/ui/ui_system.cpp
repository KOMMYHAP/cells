#include "ui_system.h"

#include "command_line.h"
#include "storage/stack_storage.h"

#include "ui_layout.h"
#include "world.h"
#include "world_widget.h"

#include "systems/id_system.h"
#include "systems/position_system.h"
#include "systems/type_system.h"

static const std::string_view FontArgument = "--font";
static const std::string_view FragmentShaderArgument = "--fragment-shader";

std::error_code UiSystem::InitializeSystem(common::StackStorage& storage)
{
    const auto& layout = storage.Get<UiLayout>();
    _window.create(sf::VideoMode(layout.screenWidth, layout.screenHeight), "Cells", sf::Style::Titlebar | sf::Style::Close);
    ASSERT(_window.isOpen());

    _window.setVerticalSyncEnabled(false);
    _window.setFramerateLimit(60);

    auto* world = storage.Get<World*>();
    const SystemRegistry& systems = world->GetSystems();

    const common::CommandLine& commandLine = storage.Get<common::CommandLine>();
    auto mbFontPath = commandLine.FindValue(FontArgument);
    ASSERT(mbFontPath.has_value(), "you should specify font path via --font argument!");

    _font = std::make_unique<sf::Font>();
    const bool fontLoaded = _font->loadFromFile(std::string{ *mbFontPath });
    ASSERT(fontLoaded, "invalid font!");

    storage.Store<UiSystem*>(this);

    {
        auto statusPanel = std::make_unique<StatusPanel>(layout, *_font, *world);
        AddWidget(std::move(statusPanel));
    }

    {
        auto mbFragmentShaderPath = commandLine.FindValue(FragmentShaderArgument);
        ASSERT(mbFragmentShaderPath.has_value());

        auto shader = std::make_unique<sf::Shader>();
        const bool loaded = shader->loadFromFile(std::string{ *mbFragmentShaderPath }, sf::Shader::Fragment);
        ASSERT(loaded);

        //        WorldWidget::Config worldRenderConfig {
        //            std::move(shader),
        //            { sf::Color::Yellow, sf::Color::White, sf::Color::White, sf::Color::White },
        //            sf::Vector2u { layout.fieldWidth, layout.fieldHeight },
        //            sf::Vector2u { layout.fieldOffset, layout.fieldOffset },
        //            systems.Modify<PositionSystem>(),
        //            systems.Modify<IdSystem>(),
        //            systems.Modify<TypeSystem>()
        //        };
        //        auto worldWidget = std::make_unique<WorldWidget>(std::move(worldRenderConfig));
        //        AddWidget(std::move(worldWidget));
    }

    return std::error_code();
}

void UiSystem::TerminateSystem()
{
    for (auto&& [_, widget] : std::ranges::reverse_view(_widgets)) {
        widget.reset();
    }
    _widgets.clear();
    _nextWidgetHandle.id = 0;
    _window.close();
}

UiSystem::MainLoopFeedback UiSystem::ProcessInput()
{
    auto feedback{ MainLoopFeedback::ShouldRun };
    sf::Event event{};
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            feedback = MainLoopFeedback::ShouldStop;
        }
    }
    return feedback;
}

void UiSystem::Update(sf::Time elapsedTime)
{
    for (auto&& [_, widget] : _widgets) {
        widget->Update(elapsedTime);
    }
}

void UiSystem::Render()
{
    if (!_window.isOpen()) {
        return;
    }

    const sf::Color gray{ 0xCCCCCCFF };
    _window.clear(gray);

    for (auto&& [_, widget] : _widgets) {
        widget->Draw(_window);
    }

    _window.display();
}

UiHandle UiSystem::AddWidget(std::unique_ptr<UiWidget> widget)
{
    const auto handle = _nextWidgetHandle;
    _widgets.emplace(handle, std::move(widget));
    _nextWidgetHandle.id += 1;
    return handle;
}

void UiSystem::RemoveWidget(UiHandle handle)
{
    ASSERT(_widgets.contains(handle));
    _widgets.erase(handle);
}