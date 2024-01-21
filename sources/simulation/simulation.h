#pragma once

#include "sample_counter.h"
#include "world.h"

class Simulation {
public:
    Simulation(World& world);

    void SetAutoMode(float ticksPerSecond, sf::Time limitSimulationTime);
    void SetManualMode();

    uint32_t Run(sf::Time elapsedTime);

    void Tick();
    void Ticks(uint32_t ticks);

    sf::Time GetTickTime() const { return sf::seconds(_tickCounter.CalcMedian()); }

private:
    void ProcessTick();

    struct RuntimeParams {
        float ticksPerSecond { 0.0f };
        sf::Time limitSimulationTime;
        float ticksToProcess { 0.0f };
    };

    World& _world;

    using TimeCounter = common::SampleCounter<float, 100>;
    TimeCounter _tickCounter;

    std::optional<RuntimeParams> _autoModeParams;
};