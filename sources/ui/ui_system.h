#pragma once

#include "registrar/registrable_system.h"
#include "widgets/root_widget.h"
#include "widgets/world/world_rasterization_system.h"

class UiSystem final : public common::RegistrableSystem {
public:
    std::error_code InitializeSystem(common::StackStorage& storage) override;
    void TerminateSystem() override;

    enum class MainLoopFeedback {
        ShouldStop,
        ShouldRun
    };
    MainLoopFeedback ProcessInput();
    void Update(sf::Time elapsedTime);
    void Render();

private:
    SDL_Window* _window { nullptr };
    SDL_Renderer* _renderer { nullptr };
    std::unique_ptr<WorldRasterizationSystem> _renderSystem;
    std::unique_ptr<RootWidget> _rootWidget;
};
