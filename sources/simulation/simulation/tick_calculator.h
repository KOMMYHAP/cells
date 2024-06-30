#pragma once

class SimulationTickCalculator {
public:
    void Setup(sf::Time targetSimulationTime, sf::Time tickTime);

    uint32_t Run(sf::Time elapsedTime);

private:
    sf::Time _tickTime;
    sf::Time _limitSimulationTime;
    sf::Time _availableTimeToSpent;
};