#pragma once

class Field;

class Simulation {
public:
    Simulation(Field& field);

    void AddTicksToUpdate(float ticksToUpdate);
    void SetAutoUpdateMode(float ticksPerSecond);

    void Update(sf::Time elapsedTime);

    struct Statistics {
        uint32_t processedTicks { 0 };
    };
    const Statistics& GetUpdateStatistics() const { return _statistics; }

private:
    void ManualUpdate();
    void AutoUpdate(sf::Time elapsedTime);

    void Tick();

    Field& _field;
    sf::Time _elapsedTime;
    bool _manualMode { true };
    float _ticksPerSecond { 0.0f };
    float _ticksToUpdate { 0.0f };
    Statistics _statistics;
};