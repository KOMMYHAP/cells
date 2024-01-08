#pragma once

class Field;

class Simulation {
public:
    Simulation(Field& field);

    void SetManualUpdateMode(uint32_t ticksToUpdate);
    void SetAutoUpdateMode(uint32_t ticksPerSecond);

    void Update(sf::Time elapsedTime);

private:
    void Tick();

    Field& _field;
    sf::Time _elapsedTime;
    bool _manualMode { true };
    uint32_t _ticksPerSecond { 0 };
    uint32_t _ticksToUpdate { 0 };
};