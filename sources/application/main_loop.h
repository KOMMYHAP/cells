#pragma once

class UiSystem;
class World;

class MainLoop {
public:
    MainLoop(UiSystem& uiSystem, World& world);

    void Run();

private:
    UiSystem& _uiSystem;
    World& _world;
    bool _stopRequested { false };
};