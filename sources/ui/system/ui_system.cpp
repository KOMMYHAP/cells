#include "ui_system.h"

#include "clock/clock.h"
#include "sdl_panic.h"
#include "world.h"

#include "widgets/menu_root_widget.h"
#include "widgets/world/world_widget.h"

#include "app_statistics.h"

UiSystem::UiSystem(const UiConfig& uiConfig)
    :_appStats(std::make_unique<AppStatistics>())
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        PanicOnSdlError("SDL_Init"sv);
    }

    static constexpr uint32_t WindowFlags = SDL_WINDOW_HIGH_PIXEL_DENSITY;
    _window = SDL_CreateWindow("Cells", uiConfig.windowSizeX, uiConfig.windowSizeY, WindowFlags);
    if (_window == nullptr) {
        PanicOnSdlError("SDL_CreateWindow"sv);
    }

    _renderer = SDL_CreateRenderer(_window, nullptr);
    if (_renderer == nullptr) {
        PanicOnSdlError("SDL_CreateRenderer"sv);
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    if (!ImGui_ImplSDL3_InitForSDLRenderer(_window, _renderer)) {
        ASSERT_FAIL("ImGui_ImplSDL2_InitForSDLRenderer failed!");
        return;
    }
    if (!ImGui_ImplSDLRenderer3_Init(_renderer)) {
        ASSERT_FAIL("ImGui_ImplSDLRenderer2_Init failed!");
        return;
    }

    _rootWidget = std::make_unique<RootWidget>();
    MenuRootWidget& menuRootWidget = _rootWidget->EmplaceWidget<MenuRootWidget>();
    _menuRootWidget = &menuRootWidget;
}

UiSystem::~UiSystem()
{
    _rootWidget.reset();
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    // ImPlot::DestroyContext();
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
        ImGui_ImplSDL3_ProcessEvent(&event);
        const bool stopByQuitEvent = event.type == SDL_EVENT_QUIT;
        const bool stopByWindowEvent = event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(_window);
        shouldStopMainLoop |= stopByQuitEvent || stopByWindowEvent;
    }

    _shouldStopMainLoop = shouldStopMainLoop;
}

void UiSystem::Update(Common::Time elapsedTime)
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    _rootWidget->UpdateWidget(elapsedTime);

    ImGui::Render();
}

void UiSystem::Render()
{
    SDL_SetRenderScale(_renderer, ImGui::GetIO().DisplayFramebufferScale.x, ImGui::GetIO().DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(_renderer, 0xCC, 0xCC, 0xCC, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(_renderer);

    _rootWidget->RenderWidget();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), _renderer);
    SDL_RenderPresent(_renderer);
}

std::unique_ptr<WorldWidget> UiSystem::MakeWorldWidget(World& world, const SDL_FRect& worldRect)
{
    return std::make_unique<WorldWidget>(world, *_renderer, worldRect);
}

void UiSystem::ApplicationRunMainLoop()
{
    Common::Clock frameClock;
    while (!_shouldStopMainLoop) {
        const Common::Time elapsedTime = frameClock.Restart();
        ProcessInput();
        Update(elapsedTime);
        Render();
        _appStats->AddFrame(elapsedTime);

        static const Common::Time TargetLoopTime{Common::Time::FromMilliseconds(15)};
        const Common::Time frameTime = _appStats->GetAverageFrameTime();
        if (frameTime < TargetLoopTime) {
            const Common::Time timeToSleep = TargetLoopTime - frameTime;
            std::this_thread::sleep_for(std::chrono::milliseconds(timeToSleep.AsMilliseconds()));
        }
    }
}