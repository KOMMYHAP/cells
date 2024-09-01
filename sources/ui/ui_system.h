#pragma once

#include "registrar/registrable_system.h"
#include "systems_ecs/render_system.h"
#include "widgets/root_widget.h"

struct SDL_Window;
struct SDL_Renderer;

class UiSystem final : public common::RegistrableSystem {
public:
    UiSystem();
    ~UiSystem();
    
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
    std::unique_ptr<RenderSystem> _renderSystem;
    std::unique_ptr<RootWidget> _rootWidget;
};
