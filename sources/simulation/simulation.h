#pragma once

#include "time_counter.h"

class Field;

class Simulation {
public:
    Simulation(Field& field);

    void SetAutoMode(float ticksPerSecond, sf::Time limitSimulationTime);
    void SetManualMode();

    uint32_t Run(sf::Time elapsedTime);

    void Tick();
    void Ticks(uint32_t ticks);

    sf::Time GetTickTime() const { return _tickCounter.GetMedianTime(); }

private:
    void ProcessTick();

    struct RuntimeParams {
        float ticksPerSecond { 0.0f };
        sf::Time limitSimulationTime;
        float ticksToProcess { 0.0f };
    };

    Field& _field;
    common::TimeCounter _tickCounter;

    std::optional<RuntimeParams> _autoModeParams;
};