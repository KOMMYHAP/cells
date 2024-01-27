#pragma once

class WorldInterface {
public:
    virtual void Tick() = 0;

protected:
    ~WorldInterface() = default;
};