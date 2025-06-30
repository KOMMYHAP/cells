#pragma once

#include "SDL3/SDL_rect.h"

#include "ui_application_interface.h"
#include "ui_config.h"
#include "widgets/root_widget.h"
#include "world.h"

class WorldWidget;
class MenuRootWidget;
struct SDL_Window;
struct SDL_Renderer;

class UiSystem final : public UiApplicationInterface {
public:
    explicit UiSystem(const UiConfig& uiConfig);
    ~UiSystem() override;

    void ApplicationRunMainLoop() override;

    void ProcessInput();
    void Update(Common::Time elapsedTime);
    void Render();

    MenuRootWidget& ModifyMenuRootWidget() { return *_menuRootWidget; }
    RootWidget& ModifyRootWidget() { return *_rootWidget; }

    std::unique_ptr<WorldWidget> MakeWorldWidget(World& world, const SDL_FRect& worldRect);

private:
    bool _shouldStopMainLoop { false };
    SDL_Window* _window { nullptr };
    SDL_Renderer* _renderer { nullptr };
    std::unique_ptr<RootWidget> _rootWidget;
    MenuRootWidget* _menuRootWidget;
};
