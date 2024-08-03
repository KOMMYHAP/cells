#pragma once

#include "registrar/registrable_system.h"

class UiSystem;
class World;

class MainLoop : public common::RegistrableSystem {
public:
    std::error_code InitializeSystem(common::StackStorage& storage) override;
    void TerminateSystem() override;

    void Run();

private:
    UiSystem* _uiSystem { nullptr };
    World* _world { nullptr };
    bool _stopRequested { false };
};