#include "reproduction_procedure.h"
#include <genetic/crossover_algorithm.h>

#include "random.h"
#include "systems/brain_system.h"
#include "systems/health_system.h"
#include "systems/position_system.h"
#include "systems/simulation_virtual_machine.h"
#include "systems/spawner.h"
#include "systems/type_system.h"

ReproductionProcedure::ReproductionProcedure(const SimulationVirtualMachine& vm, PositionSystem& positionSystem, HealthSystem& healthSystem, BrainSystem& brainSystem, TypeSystem& typeSystem, Spawner& spawner)
    : _vm(vm)
    , _positionSystem(positionSystem)
    , _healthSystem(healthSystem)
    , _brainSystem(brainSystem)
    , _typeSystem(typeSystem)
    , _spawner(spawner)
{
}

void ReproductionProcedure::Execute(ProcedureContext& context)
{
    const auto [readArgs, rawDirection] = context.TryPopArgs<uint8_t>();
    if (!readArgs) {
        return;
    }
    Direction direction;
    if (!TryParse(rawDirection, direction)) {
        context.MarkProcedureAsInvalid();
        return;
    }
    const CellId id = _vm.GetRunningCellId();

    constexpr CellHealth healthPerAction { 5 };
    if (_healthSystem.Decrement(id, healthPerAction) == CellHealth::Zero) {
        return;
    }

    const CellPosition position = _positionSystem.Get(id);
    const CellPosition secondParentPosition = _positionSystem.TryApplyDirection(position, direction);
    if (secondParentPosition == InvalidCellPosition) {
        return;
    }

    const CellId secondParentId = _positionSystem.Find(secondParentPosition);
    const CellType secondParentType = _typeSystem.Get(secondParentId);
    if (secondParentType != CellType::Unit) {
        // Hmm... Does cell try to reproduce with ... food?
        return;
    }

    const CellPosition childPosition = SelectPosition(position, secondParentPosition);
    if (childPosition == InvalidCellPosition) {
        return;
    }

    constexpr CellHealth childInitialHealth { 50 };

    SpawnProperties properties;
    properties.position = childPosition;
    properties.health = childInitialHealth;
    properties.type = CellType::Unit;
    properties.brain = MakeChildBrain(id, secondParentId);
    _spawner.TrySpawn(properties);
}

CellPosition ReproductionProcedure::SelectPosition(CellPosition lhs, CellPosition rhs) const
{
    assert(_positionSystem.IsNeighbourFor(lhs, rhs));

    const int16_t fromX = std::min(lhs.x, rhs.x);
    const int16_t fromY = std::min(lhs.y, rhs.y);
    const int16_t toX = std::min(lhs.x, rhs.x);
    const int16_t toY = std::min(lhs.y, rhs.y);

    std::array<CellPosition, 10> testPositions;
    uint8_t testPositionIdx { 0 };

    for (int16_t y = fromY; y < toY; ++y) {
        for (int16_t x = fromX; x < toX; ++x) {
            const CellPosition position { x, y };
            if (position == lhs || position == rhs) {
                continue;
            }
            testPositions[testPositionIdx] = position;
            testPositionIdx += 1;
        }
    }

    std::uniform_int_distribution<uint16_t> offsetDistribution { 0, testPositions.size() - 1 };
    const uint8_t offset = static_cast<uint8_t>(offsetDistribution(common::GetRandomEngine()));

    for (uint8_t i = 0; i < testPositions.size(); ++i) {
        const uint8_t index = (i + offset) % testPositions.size();
        const CellPosition position = testPositions[index];
        const CellId id = _positionSystem.Find(position);
        const bool emptyPosition = id == CellId::Invalid;
        if (emptyPosition) {
            return position;
        }
    }

    return InvalidCellPosition;
}

CellBrain ReproductionProcedure::MakeChildBrain(CellId lhs, CellId rhs) const
{
    constexpr uint8_t crossoverPoint = 10;
    CrossoverAlgorithm crossover { crossoverPoint };
    return crossover.Combine(_brainSystem.Get(lhs), _brainSystem.Get(rhs));
}
