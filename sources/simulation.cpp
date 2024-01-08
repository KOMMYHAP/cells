#include "simulation.h"
#include "brain.h"
#include "field.h"

Simulation::Simulation(Field& field)
    : _field(field)
{
}

void Simulation::Update(sf::Time elapsedTime)
{
    _elapsedTime += elapsedTime;

    const float ticksPerDiff = _elapsedTime.asSeconds() * _ticksPerSecond;
    const uint32_t ticksToProcess = std::round(ticksPerDiff);

    if (ticksToProcess == 0) {
        return;
    }

    const float processedTime = ticksToProcess / static_cast<float>(_ticksPerSecond);
    _elapsedTime -= sf::seconds(processedTime);

    for (uint32_t i { 0 }; i < ticksToProcess; ++i) {
        Tick();
    }
}

void Simulation::Tick()
{
    for (Cell& cell : _field) {
        Brain brain { cell };
        brain.Process();
    }
}
void Simulation::SetManualUpdateMode(uint32_t ticksToUpdate)
{
    _manualMode = true;
    _ticksToUpdate = ticksToUpdate;
}

void Simulation::SetAutoUpdateMode(uint32_t ticksPerSecond)
{
    _manualMode = false;
    _ticksPerSecond = ticksPerSecond;
}
