#include "world.h"

#include "SFML/Graphics/Shader.hpp"
#include "procedures/move_procedure.h"
#include "procedures/simulation_procedure_context.h"

#include "systems_ecs/brain_simulation_system.h"
#include "systems_ecs/movement_system.h"
#include "systems_ecs/spawn_system.h"

World::World()
    : _worldSize(100, 100)
    , _cellsOnCurrentTick(_worldSize.x, _worldSize.y)
    , _cellsOnNextTick(_worldSize.x, _worldSize.y)
    , _randomEngine(Random::MakeEngine("white"))
{
    const sf::Time targetSimulationTime = sf::milliseconds(30);

    _simulationVm.RegisterProcedure<MoveProcedure>(ProcedureType::Move, 1, 0, "move", _ecsWorld);

    _simulationVm.SetInstructionsPerStep(1);
    _simulationVm.SetWatcher([this](const ProcessorState state, const ProcessorExternalContext& context) {
        if (state == ProcessorState::Good) {
            return;
        }
        const CellId id = context.Get<SimulationProcedureContext>().id;
        _ecsWorld.destroy(id);
    });

    _simulationSystems.emplace_back(std::make_unique<MovementSystem>(_ecsWorld, _cellsOnCurrentTick, _cellsOnNextTick));
    _simulationSystems.emplace_back(std::make_unique<SpawnSystem>(_ecsWorld, Random::Accessor { _randomEngine }, _simulationVm));
    _simulationSystems.emplace_back(std::make_unique<BrainSimulationSystem>(_ecsWorld, _simulationVm));

    auto createCell = [this](int16_t x, int16_t y) {
        const CellId id = _ecsWorld.create();
        _ecsWorld.emplace<SpawnPlace>(id, SpawnPlace {});
        _ecsWorld.emplace<CellPosition>(id, CellPosition { x, y });
    };
    for (int i = 0; i < 100; ++i) {
        int x = i % _worldSize.x;
        int y = i % _worldSize.y;
        createCell(x, y);
    }

    _tickCalculator.Setup(targetSimulationTime, sf::seconds(1));
}

void World::Update(const sf::Time elapsedTime)
{
    const uint32_t ticks = _tickCalculator.Run(elapsedTime);
    for (uint32_t i { 0 }; i < ticks; ++i) {
        for (const auto& system : _simulationSystems) {
            system->DoSystemUpdate();
        }
        _cellsOnCurrentTick = _cellsOnNextTick;
    }
}
