#pragma once
#include "simulation_system.h"

class BaseScript;

class Simulation {
public:
    void Setup(sf::Time targetSimulationTime);

    uint32_t Run(sf::Time elapsedTime);

private:
    void Ticks(uint32_t count);

    sf::Time _tickTime { sf::microseconds(10) };
    sf::Time _limitSimulationTime;
    sf::Time _availableTimeToSpent;
};