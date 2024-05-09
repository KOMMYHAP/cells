#pragma once

#include "sample_counter.h"
#include "updatable.h"

class BaseScript;

class Simulation : public Updatable {
public:
    Simulation(BaseScript& tickScript);

    void SetAutoMode(sf::Time targetSimulationTime);
    void SetManualMode();

    uint32_t Run(sf::Time elapsedTime);

    void Tick();
    void Ticks(uint32_t ticks);

public:
    void Update(sf::Time elapsedTime) override;

private:
    void ProcessTick();
    uint32_t WarmUp();

    struct RuntimeParams {
        sf::Time limitSimulationTime;
        sf::Time availableTimeToSpent;
    };

    BaseScript& _tickScript;

    using TimeCounter = common::SampleCounter<float, 10>;
    TimeCounter _tickCounter;

    std::optional<RuntimeParams> _autoModeParams;
};