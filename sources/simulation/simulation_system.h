#pragma once

class SimulationSystem {
public:
    virtual ~SimulationSystem() = default;
    virtual void Update() = 0;
};