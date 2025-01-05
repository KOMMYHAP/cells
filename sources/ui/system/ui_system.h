#pragma once

#include "ui_application_interface.h"
#include "ui_config.h"
#include "widgets/root_widget.h"
#include "widgets/world/world_rasterization_system.h"
#include "world.h"

class MenuRootWidget;
class UiSystem final : public UiApplicationInterface {
public:
    explicit UiSystem(World& world, const UiConfig& uiConfig);
    ~UiSystem() override;

    void ApplicationRunMainLoop() override;

    MenuRootWidget& GetMenuRootWidget() { return *_menuRootWidget; }

private:
    void ProcessInput();
    void Update(Common::Time elapsedTime);
    void Render();

    bool _shouldStopMainLoop { false };
    SDL_Window* _window { nullptr };
    SDL_Renderer* _renderer { nullptr };
    std::unique_ptr<WorldRasterizationSystem> _renderSystem;
    std::unique_ptr<RootWidget> _rootWidget;
    MenuRootWidget* _menuRootWidget;
};
