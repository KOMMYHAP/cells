#include "reproduction_procedure.h"
#include <genetic/crossover_algorithm.h>

#include "simulation_procedure_context.h"
#include "systems_ecs/cell_locator.h"
#include "systems_ecs/simulation_virtual_machine.h"

ReproductionProcedure::ReproductionProcedure(const SimulationVirtualMachine& vm, CellLocator& positionSystem, HealthSystem& healthSystem, BrainSystem& brainSystem, TypeSystem& typeSystem, Spawner& spawner)
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
    ASSERT_FAIL("Not implemented!");
    const auto [readArgs, rawDirection] = context.TryPopArgs<uint8_t>();
    if (!readArgs) {
        return;
    }
    Direction direction;
    if (!TryParseDirection(rawDirection, direction)) {
        context.MarkProcedureAsInvalid();
        return;
    }
    const CellId id = context.GetExternalContext().Get<SimulationProcedureContext>().id;

    // constexpr CellHealth healthPerAction { 50 };
    // if (_healthSystem.Decrement(id, healthPerAction) == CellHealth::Zero) {
    //     return;
    // }

    // const CellPosition position = _positionSystem.Get(id);
    // const CellPosition secondParentPosition = _positionSystem.TryApplyDirection(position, direction);
    // if (secondParentPosition == InvalidCellPosition) {
    //     return;
    // }
    //
    // const CellId secondParentId = _positionSystem.Find(secondParentPosition);
    // if (secondParentId == CellId::Invalid) {
    //     return;
    // }
    // const CellType secondParentType = _typeSystem.Get(secondParentId);
    // if (secondParentType != CellType::Unit) {
    //     // Hmm... Does cell try to reproduce with ... food?
    //     return;
    // }

    // const CellPosition childPosition = SelectPosition(position, secondParentPosition);
    // if (childPosition == InvalidCellPosition) {
    //     return;
    // }

    // constexpr CellHealth childInitialHealth { 35 };
    //
    // SpawnProperties properties;
    // properties.position = childPosition;
    // properties.health = childInitialHealth;
    // properties.type = CellType::Unit;
    // properties.brain = MakeChildBrain(id, secondParentId);
    // _spawner.TrySpawn(properties);
}

CellPosition ReproductionProcedure::SelectPosition(CellPosition lhs, CellPosition rhs) const
{
    ASSERT(_positionSystem.IsNeighbourFor(lhs, rhs));

    const auto fromX = NarrowCast<int16_t>(std::min(lhs.x - 1, rhs.x - 1));
    const auto fromY = NarrowCast<int16_t>(std::min(lhs.y - 1, rhs.y - 1));
    const auto toX = NarrowCast<int16_t>(std::max(lhs.x + 1, rhs.x + 1));
    const auto toY = NarrowCast<int16_t>(std::max(lhs.y + 1, rhs.y + 1));

    std::array<CellPosition, 6> testPositions;
    uint8_t testPositionSize { 0 };

    for (auto y = fromY; y <= toY; ++y) {
        for (auto x = fromX; x <= toX; ++x) {
            const CellPosition position { x, y };

            if (position == lhs || position == rhs) {
                continue;
            }
            if (!_positionSystem.IsValidPosition(position)) {
                continue;
            }
            const bool isNeighbour = _positionSystem.IsNeighbourFor(lhs, position) || _positionSystem.IsNeighbourFor(rhs, position);
            ASSERT(isNeighbour);
            // if (!isNeighbour) {
            //     continue;
            // }

            ASSERT(testPositionSize < testPositions.size());
            testPositions[testPositionSize] = position;
            testPositionSize += 1;
        }
    }

    ASSERT(testPositionSize >= 1);
    //
    // std::uniform_int_distribution<uint16_t> offsetDistribution { 0, static_cast<uint16_t>(testPositionSize - 1) };
    // const uint8_t offset = static_cast<uint8_t>(offsetDistribution(common::GetRandomEngine()));
    //
    // for (uint8_t i = 0; i < testPositionSize; ++i) {
    //     const uint8_t index = (i + offset) % testPositionSize;
    //     const CellPosition position = testPositions[index];
    //     const CellId id = _positionSystem.Find(position);
    //     const bool emptyPosition = id == CellId::Invalid;
    //     if (emptyPosition) {
    //         return position;
    //     }
    // }

    return InvalidCellPosition;
}

CellBrain ReproductionProcedure::MakeChildBrain(CellId lhs, CellId rhs) const
{
    // constexpr uint8_t crossoverPoint = 10;
    // CrossoverAlgorithm crossover { crossoverPoint };
    // return crossover.Combine(_brainSystem.Get(lhs), _brainSystem.Get(rhs));
    return {};
}