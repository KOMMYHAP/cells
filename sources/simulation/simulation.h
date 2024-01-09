#pragma once

class Field;

class Simulation {
public:
    Simulation(Field& field);

    void SetManualUpdateMode(uint32_t ticksToUpdate);
    void SetAutoUpdateMode(uint32_t ticksPerSecond);

    void Update(sf::Time elapsedTime);

    sf::Time GetTickProcessingTime() const { return _tickProcessingTime; }

private:
    void ManualUpdate();
    void AutoUpdate(sf::Time elapsedTime);

    void Tick();

    Field& _field;
    sf::Time _elapsedTime;
    sf::Time _tickProcessingTime;
    bool _manualMode { true };
    uint32_t _ticksPerSecond { 0 };
    uint32_t _ticksToUpdate { 0 };
};