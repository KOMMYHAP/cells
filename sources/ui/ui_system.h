#pragma once

#include "registrar/registrable_system.h"
#include "root_widget.h"
#include "status_panel.h"
#include "systems_ecs/render_system.h"
#include "ui_widget.h"

class UiSystem : public common::RegistrableSystem {
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

    const sf::Font& GetDefaultFont() const { return *_font; }

private:
    sf::RenderWindow _window;
    std::unique_ptr<sf::Font> _font;
    std::unique_ptr<RenderSystem> _renderSystem;
    std::unique_ptr<RootWidget> _rootWidget;
};
