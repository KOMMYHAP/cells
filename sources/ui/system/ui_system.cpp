#include "ui_system.h"

#include "storage/stack_storage.h"

#include "sdl_panic.h"
#include "ui_layout.h"
#include "world.h"

#include "menu_widgets/fps_widget.h"
#include "menu_widgets/group_menu_widget.h"
#include "menu_widgets/imgui_demo_menu_widget.h"
#include "menu_widgets/implot_demo_menu_widget.h"
#include "widgets/menu_root_widget.h"
#include "widgets/world/world_rasterization_system.h"
#include "widgets/world/world_widget.h"

UiSystem::UiSystem(World& world)
{
    UiLayout layout;
    layout.screenWidth = 800;
    layout.screenHeight = 600;
    layout.fieldOffset = 20;
    layout.fieldWidth = layout.screenWidth - 2 * layout.fieldOffset;
    layout.fieldHeight = layout.screenHeight - 2 * layout.fieldOffset;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        PanicOnSdlError("SDL_Init"sv);
    }

    static constexpr auto WindowFlags = static_cast<SDL_WindowFlags>(SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    _window = SDL_CreateWindow("Cells", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, layout.screenWidth, layout.screenHeight, WindowFlags);
    if (_window == nullptr) {
        PanicOnSdlError("SDL_CreateWindow"sv);
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (_renderer == nullptr) {
        PanicOnSdlError("SDL_CreateRenderer"sv);
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    if (!ImGui_ImplSDL2_InitForSDLRenderer(_window, _renderer)) {
        ASSERT_FAIL("ImGui_ImplSDL2_InitForSDLRenderer failed!");
        return;
    }
    if (!ImGui_ImplSDLRenderer2_Init(_renderer)) {
        ASSERT_FAIL("ImGui_ImplSDLRenderer2_Init failed!");
        return;
    }

    _rootWidget = std::make_unique<RootWidget>();

    EcsWorld& ecsWorld = world.ModifyEcsWorld();
    _renderSystem = std::make_unique<WorldRasterizationSystem>(ecsWorld);
    const SDL_Rect worldRect { layout.fieldOffset, layout.fieldOffset, layout.fieldWidth, layout.fieldHeight };
    _rootWidget->AddWidget<WorldWidget>(*_renderer, world, *_renderSystem, worldRect);

    MenuRootWidget& menuRootWidget = _rootWidget->AddWidget<MenuRootWidget>();
    const MenuWidgetId gameMenu = menuRootWidget.AddWidget<GroupMenuWidget>("Game");
    menuRootWidget.AddWidget<FpsWidget>(gameMenu, "FPS");
    menuRootWidget.AddWidget<ImGuiDemoMenuWidget>("ImGui Demo");
    menuRootWidget.AddWidget<ImPlotDemoMenuWidget>("ImPlot Demo");
}

UiSystem::~UiSystem()
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

void UiSystem::ProcessInput()
{
    bool shouldStopMainLoop = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        const bool stopByQuitEvent = event.type == SDL_QUIT;
        const bool stopByWindowEvent = event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(_window);
        shouldStopMainLoop |= stopByQuitEvent || stopByWindowEvent;
    }

    _shouldStopMainLoop = shouldStopMainLoop;
}

void UiSystem::StartFrame()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
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

void UiSystem::EndFrame()
{
}

void UiSystem::ApplicationRunMainLoop()
{
    sf::Clock frameClock;
    while (!_shouldStopMainLoop) {
        const sf::Time elapsedTime = frameClock.restart();
        ProcessInput();
        StartFrame();
        Update(elapsedTime);
        Render();
        EndFrame();
    }
}