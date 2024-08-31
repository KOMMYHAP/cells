#include "ui_system.h"

#include "command_line.h"
#include "storage/stack_storage.h"

#include "ui_layout.h"
#include "world.h"

#include "utils/stub_error_code.h"
#include "widgets/fps_widget.h"
#include "widgets/world_widget.h"

static constexpr std::string_view FontArgument = "--font";
static constexpr std::string_view FragmentShaderArgument = "--fragment-shader";

std::error_code UiSystem::InitializeSystem(common::StackStorage& storage)
{
    UiLayout layout;
    layout.screenWidth = 800;
    layout.screenHeight = 600;
    layout.fieldOffset = 20;
    layout.fieldWidth = layout.screenWidth - 2 * layout.fieldOffset;
    layout.fieldHeight = layout.screenHeight - 2 * layout.fieldOffset;
    layout.statusTextOffset = 5;
    layout.statusTextSize = 10;
    layout.cellPadding = 0;

    // ASSERT(StatusTextOffset * 2 + StatusTextSize <= FieldOffset);
    // ASSERT(layout.fieldWidth % (CellSize + CellPadding) == 0);
    // ASSERT(layout.fieldHeight % (CellSize + CellPadding) == 0);

    _window.create(sf::VideoMode(layout.screenWidth, layout.screenHeight), "Cells", sf::Style::Titlebar | sf::Style::Close);
    ASSERT(_window.isOpen());

    _window.setVerticalSyncEnabled(false);
    _window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(_window)) {
        ASSERT_FAIL("Failed to init ImGui SFML!");
        return common::MakeStubErrorCode();
    }

    auto& world = storage.Modify<World>();

    const auto& commandLine = storage.Get<common::CommandLine>();
    auto mbFontPath = commandLine.FindValue(FontArgument);
    if (!mbFontPath.has_value()) {
        ASSERT_FAIL("Stub: pass font path to cmd line!");
        return common::MakeStubErrorCode();
    }

    _font = std::make_unique<sf::Font>();
    const bool fontLoaded = _font->loadFromFile(std::string { *mbFontPath });
    if (!fontLoaded) {
        ASSERT_FAIL("Stub: invalid font by specified path!");
        return common::MakeStubErrorCode();
    }

    storage.Store<UiSystem*>(this);

    // {
    //     auto statusPanel = std::make_unique<StatusPanel>(layout, *_font, world);
    //     AddWidget(std::move(statusPanel));
    // }

    EcsWorld& ecsWorld = world.ModifyEcsWorld();
    _renderSystem = std::make_unique<RenderSystem>(ecsWorld, world.GetWorldSize());

    _rootWidget = std::make_unique<RootWidget>(_window);
    {
        auto mbFragmentShaderPath = commandLine.FindValue(FragmentShaderArgument);
        ASSERT(mbFragmentShaderPath.has_value());

        auto shader = std::make_unique<sf::Shader>();
        const bool loaded = shader->loadFromFile(std::string { *mbFragmentShaderPath }, sf::Shader::Fragment);
        ASSERT(loaded);

        WorldWidget::Config worldRenderConfig {
            _renderSystem.get(),
            std::move(shader),
            sf::Vector2u { layout.fieldWidth, layout.fieldHeight },
            sf::Vector2u { layout.fieldOffset, layout.fieldOffset },
        };
        _rootWidget->AddWidget<WorldWidget>(std::move(worldRenderConfig));
    }

    _rootWidget->AddWidget<FpsWidget>();

    return {};
}

void UiSystem::TerminateSystem()
{
    _rootWidget.reset();
    _renderSystem.reset();
    _font.reset();
    ImGui::SFML::Shutdown();
    _window.close();
}

UiSystem::MainLoopFeedback UiSystem::ProcessInput()
{
    auto feedback { MainLoopFeedback::ShouldRun };
    sf::Event event {};
    while (_window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(_window, event);
        if (event.type == sf::Event::Closed) {
            feedback = MainLoopFeedback::ShouldStop;
        }
    }
    return feedback;
}

void UiSystem::Update(sf::Time elapsedTime)
{
    _rootWidget->UpdateWidget(elapsedTime);
}

void UiSystem::Render()
{
    if (!_window.isOpen()) {
        return;
    }

    const sf::Color gray { 0xCCCCCCFF };
    _window.clear(gray);
    _rootWidget->RenderWidget(_window);
    _window.display();
}