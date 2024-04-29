#pragma once

#include "sample_counter.h"

class WorldInterface;

class Simulation {
public:
    Simulation(WorldInterface& world);

    void SetAutoMode(sf::Time targetSimulationTime);
    void SetManualMode();

    uint32_t Run(sf::Time elapsedTime);

    void Tick();
    void Ticks(uint32_t ticks);

private:
    void ProcessTick();
    uint32_t WarmUp();

    struct RuntimeParams {
        sf::Time limitSimulationTime;
        sf::Time availableTimeToSpent;
    };

    WorldInterface& _world;

    using TimeCounter = common::SampleCounter<float, 10>;
    TimeCounter _tickCounter;

    std::optional<RuntimeParams> _autoModeParams;
};