#include "simulation.h"
#include "brain/brain.h"
#include "brain/brain_processor.h"
#include "field/field.h"
#include "field/field_iterator.h"
#include "simulation_profile_category.h"

Simulation::Simulation(Field& field)
    : _field(field)
{
}

void Simulation::Update(sf::Time elapsedTime)
{
    common::ProfileScope updateProfileScope { "Update", SimulationProfileCategory };

    if (_manualMode) {
        ManualUpdate();
    } else {
        AutoUpdate(elapsedTime);
    }
}

void Simulation::Tick()
{
    common::ProfileScope tickProfileScope { "Tick", SimulationProfileCategory };

    _field.IterateByData([this](const CellId id, Cell& cell) {
        Brain brain { cell };

        switch (brain.GetInfo().type) {
        case CellType::Unit: {
            BrainProcessor processor { id, brain, _field };
            processor.Process();
        } break;
        case CellType::Food:
            break;
        case CellType::Wall:
            break;
        case CellType::Dummy:
            break;
        }
    });
}
void Simulation::AddTicksToUpdate(float ticksToUpdate)
{
    assert(ticksToUpdate >= 0.0f);
    _manualMode = true;
    _ticksToUpdate += ticksToUpdate;
}

void Simulation::SetAutoUpdateMode(float ticksPerSecond)
{
    assert(ticksPerSecond >= 0.0f);
    _manualMode = false;
    _ticksPerSecond = ticksPerSecond;
}

void Simulation::ManualUpdate()
{
    const uint32_t ticksToUpdate = static_cast<uint32_t>(std::floor(_ticksToUpdate));
    for (uint32_t i { 0 }; i < ticksToUpdate; ++i) {
        Tick();
    }
    _statistics.processedTicks = ticksToUpdate;
    _ticksToUpdate -= static_cast<float>(ticksToUpdate);
}

void Simulation::AutoUpdate(sf::Time elapsedTime)
{
    _elapsedTime += elapsedTime;

    const float ticksPerDiff = _elapsedTime.asSeconds() * _ticksPerSecond;
    const uint32_t ticksToProcess = std::floor(ticksPerDiff);

    if (ticksToProcess == 0) {
        return;
    }

    const float processedTime = ticksToProcess / _ticksPerSecond;
    _elapsedTime -= sf::seconds(processedTime);

    _ticksToUpdate = ticksToProcess;
    ManualUpdate();
}