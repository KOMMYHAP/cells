#pragma once

class SimulationSystem {
public:
    virtual ~SimulationSystem() = default;
    virtual void DoSystemUpdate() = 0;
};
