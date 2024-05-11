#pragma once

#include "registrar/registrable_system.h"
#include "status_panel.h"

class WorldRender;

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

private:
    sf::RenderWindow _window;
    sf::RenderStates _worldStates;
    WorldRender* _worldRender { nullptr };
    std::unique_ptr<sf::Font> _font;
    std::unique_ptr<StatusPanel> _statusPanel;
};
