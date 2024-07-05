#include "world.h"

#include "SFML/Graphics/Shader.hpp"
#include "cell_factories/patrol_cell.h"
#include "components/cell_type.h"
#include "procedures/move_procedure.h"
#include "procedures/simulation_procedure_context.h"

#include "systems_ecs/brain_simulation_system.h"
#include "systems_ecs/look_system.h"
#include "systems_ecs/movement_system.h"
#include "systems_ecs/reproduction_system.h"
#include "systems_ecs/spawn_system.h"

World::World()
    : _worldSize(100, 100)
    , _cellsLocator(_worldSize.x, _worldSize.y)
    , _randomEngine(Random::MakeEngine("white"))
{
    const sf::Time targetSimulationTime = sf::milliseconds(30);

    _simulationVm.RegisterProcedure<MoveProcedure>(ProcedureType::Move, 1, 0, "move", _ecsWorld);

    _simulationVm.SetInstructionsPerStep(8);
    _simulationVm.SetWatcher([this](const ProcessorState state, const ProcessorExternalContext& context) {
        if (state == ProcessorState::Good) {
            return;
        }
        const CellId id = context.Get<SimulationProcedureContext>().id;
        _ecsWorld.destroy(id);
    });

    _simulationSystems.emplace_back(std::make_unique<MovementSystem>(_ecsWorld, _cellsLocator));
    _simulationSystems.emplace_back(std::make_unique<LookSystem>(_ecsWorld, _cellsLocator));
    _simulationSystems.emplace_back(std::make_unique<ReproductionSystem>(_ecsWorld, _simulationVm, _cellsLocator, Random::Accessor { _randomEngine }));
    _simulationSystems.emplace_back(std::make_unique<BrainSimulationSystem>(_ecsWorld, _simulationVm));

    auto createCell = [this](int16_t x, int16_t y) {
        const CellId child = _ecsWorld.create();
        _ecsWorld.emplace<CellBrain>(child, MakePatrolCell(_simulationVm));
        _ecsWorld.emplace<CellPosition>(child, x, y);
    };
    for (int i = 0; i < 100; ++i) {
        int x = i % _worldSize.x;
        int y = i % _worldSize.y;
        createCell(x, y);
    }
    _tickCalculator.Setup(targetSimulationTime);
}

void World::Update(const sf::Time elapsedTime)
{
    Warmup();

    const uint32_t ticks = _tickCalculator.CalculateElapsedTicks(GetTickTime(), elapsedTime);
    for (uint32_t i { 0 }; i < ticks; ++i) {
        Tick();
    }
}

void World::Warmup()
{
    while (!_tickSampler.IsFull()) {
        Tick();
    }
}

sf::Time World::GetTickTime() const
{
    return sf::seconds(1);
    // return sf::seconds(_tickSampler.CalcMedian());
}

void World::Tick()
{
    const sf::Clock clock;
    for (const auto& system : _simulationSystems) {
        system->DoSystemUpdate();
    }
    const float seconds = clock.getElapsedTime().asSeconds();
    _tickSampler.AddSample(seconds);
}
