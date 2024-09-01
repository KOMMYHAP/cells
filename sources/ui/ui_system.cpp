#include "ui_system.h"

#include "SDL2/SDL.h"

#include "command_line.h"
#include "storage/stack_storage.h"

#include "ui_layout.h"
#include "utils/stub_error_code.h"
#include "world.h"

#include "menu_widgets/fps_widget.h"
#include "menu_widgets/group_menu_widget.h"
#include "menu_widgets/imgui_demo_menu_widget.h"
#include "menu_widgets/implot_demo_menu_widget.h"
#include "widgets/menu_root_widget.h"
#include "widgets/world/world_rasterization_system.h"
#include "widgets/world/world_widget.h"

static constexpr std::string_view FontArgument = "--font";

std::error_code UiSystem::InitializeSystem(common::StackStorage& storage)
{
    UiLayout layout;
    layout.screenWidth = 800;
    layout.screenHeight = 600;
    layout.fieldOffset = 20;
    layout.fieldWidth = layout.screenWidth - 2 * layout.fieldOffset;
    layout.fieldHeight = layout.screenHeight - 2 * layout.fieldOffset;
    layout.cellPadding = 0;

    // ASSERT(StatusTextOffset * 2 + StatusTextSize <= FieldOffset);
    // ASSERT(layout.fieldWidth % (CellSize + CellPadding) == 0);
    // ASSERT(layout.fieldHeight % (CellSize + CellPadding) == 0);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        std::cerr << std::format("SDL_Init failed: %s!", SDL_GetError()) << std::endl;
        return common::MakeStubErrorCode();
    }

    static constexpr auto WindowFlags = static_cast<SDL_WindowFlags>(SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    _window = SDL_CreateWindow("Cells", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, layout.screenWidth, layout.screenHeight, WindowFlags);
    if (_window == nullptr) {
        std::cerr << std::format("SDL_CreateWindow failed: %s!", SDL_GetError()) << std::endl;
        return common::MakeStubErrorCode();
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (_renderer == nullptr) {
        std::cerr << std::format("SDL_CreateRenderer failed: %s!", SDL_GetError()) << std::endl;
        return common::MakeStubErrorCode();
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    ImGui_ImplSDL2_InitForSDLRenderer(_window, _renderer);
    ImGui_ImplSDLRenderer2_Init(_renderer);

    auto& world = storage.Modify<World>();

    const auto& commandLine = storage.Get<common::CommandLine>();
    auto mbFontPath = commandLine.FindValue(FontArgument);
    if (!mbFontPath.has_value()) {
        ASSERT_FAIL("Stub: pass font path to cmd line!");
        return common::MakeStubErrorCode();
    }

    storage.Store<UiSystem*>(this);

    _rootWidget = std::make_unique<RootWidget>();

    EcsWorld& ecsWorld = world.ModifyEcsWorld();
    _renderSystem = std::make_unique<WorldRasterizationSystem>(ecsWorld);
    const SDL_Rect worldRect { layout.fieldOffset, layout.fieldOffset, layout.fieldWidth, layout.fieldHeight };
    _rootWidget->AddWidget<WorldWidget>(*_renderer, *_renderSystem, worldRect);

    MenuRootWidget& menuRootWidget = _rootWidget->AddWidget<MenuRootWidget>();
    const MenuWidgetId gameMenu = menuRootWidget.AddWidget<GroupMenuWidget>("Game");
    menuRootWidget.AddWidget<FpsWidget>(gameMenu, "FPS");
    menuRootWidget.AddWidget<ImGuiDemoMenuWidget>("ImGui Demo");
    menuRootWidget.AddWidget<ImPlotDemoMenuWidget>("ImPlot Demo");

    return {};
}

void UiSystem::TerminateSystem()
{
    _rootWidget.reset();
    _renderSystem.reset();
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

UiSystem::MainLoopFeedback UiSystem::ProcessInput()
{
    auto feedback { MainLoopFeedback::ShouldRun };
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        const bool stopByQuitEvent = event.type == SDL_QUIT;
        const bool stopByWindowEvent = event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(_window);
        feedback = (stopByQuitEvent || stopByWindowEvent) ? MainLoopFeedback::ShouldStop : MainLoopFeedback::ShouldRun;
    }

    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    return feedback;
}

void UiSystem::Update(sf::Time elapsedTime)
{
    _rootWidget->UpdateWidget(elapsedTime);
}

void UiSystem::Render()
{
    _rootWidget->RenderWidget();
    ImGui::Render();

    SDL_RenderSetScale(_renderer, ImGui::GetIO().DisplayFramebufferScale.x, ImGui::GetIO().DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(_renderer, 0xCC, 0xCC, 0xCC, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(_renderer);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(_renderer);
}