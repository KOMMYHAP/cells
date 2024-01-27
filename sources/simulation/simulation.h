#pragma once

#include "sample_counter.h"

class WorldInterface;

class Simulation {
public:
    Simulation(WorldInterface& world);

    void SetAutoMode(float ticksPerSecond, sf::Time limitSimulationTime);
    void SetManualMode();

    uint32_t Run(sf::Time elapsedTime);

    void Tick();
    void Ticks(uint32_t ticks);

    sf::Time GetTickTime() const;

private:
    void ProcessTick();

    struct RuntimeParams {
        float ticksPerSecond { 0.0f };
        sf::Time limitSimulationTime;
        float ticksToProcess { 0.0f };
    };

    WorldInterface& _world;

    using TimeCounter = common::SampleCounter<float, 100>;
    TimeCounter _tickCounter;

    std::optional<RuntimeParams> _autoModeParams;
};