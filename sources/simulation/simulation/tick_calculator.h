#pragma once

class SimulationTickCalculator {
public:
    void Setup(sf::Time targetSimulationTime);

    uint32_t Run(sf::Time elapsedTime);

private:
    sf::Time _tickTime { sf::microseconds(10) };
    sf::Time _limitSimulationTime;
    sf::Time _availableTimeToSpent;
};