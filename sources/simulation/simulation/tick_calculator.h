#pragma once

class SimulationTickCalculator {
public:
    void Setup(sf::Time targetSimulationTime);

    uint32_t CalculateElapsedTicks(sf::Time tickTime, sf::Time elapsedTime);

private:
    sf::Time _limitSimulationTime;
    sf::Time _availableTimeToSpent;
};