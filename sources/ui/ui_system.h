#pragma once

#include "drawable.h"
#include "registrar/registrable_system.h"

class WorldWidget;

class UiSystem : public common::RegistrableSystem {
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
    void Render();

private:
    sf::RenderWindow _window;
    std::unique_ptr<WorldWidget> _worldWidget;
};
