#include "random_cell_spawn_procedure_system.h"

#include "components/generated/cell_energy_change.h"
#include "components/generated/direction.h"
#include "procedures/procedure_context.h"

#include "simulation/cell_locator.h"

RandomCellSpawnProcedureSystem::RandomCellSpawnProcedureSystem(EcsWorld& world, SimulationVirtualMachine& vm, const CellLocator& locator, Spawner& spawner, RandomCellFactory& factory)
    : _world(&world)
    , _vm(&vm)
    , _locator(&locator)
    , _spawner(&spawner)
    , _factory(&factory)
{
}
RandomCellSpawnProcedureSystem::ExecutionStatus RandomCellSpawnProcedureSystem::ExecuteProcedure(EcsEntity /*id*/, ProcedureContext& context, CellBrain& /*brain*/, CellPosition position, CellEnergyDecrease& energyChange)
{
    const auto [readArgs, rawDirection] = context.TryPopArgs<uint8_t>();
    if (!readArgs) {
        return ExecutionStatus::Error;
    }
    Direction direction;
    if (!TryParseDirection(rawDirection, direction)) {
        return ExecutionStatus::Error;
    }

    const EcsEntity childId = _spawner->ScheduleSpawn(position);
    CellBrain& childBrain = _world->emplace<CellBrain>(childId);
    _factory->Make(childBrain);

    energyChange.value -= 50;
    return ExecutionStatus::Success;
}

// CellPosition ReproductionProcedure::SelectPosition(CellPosition lhs, CellPosition rhs) const
// {
//     ASSERT(_positionSystem.IsNeighbourFor(lhs, rhs));
//
//     const auto fromX = NarrowCast<int16_t>(std::min(lhs.x - 1, rhs.x - 1));
//     const auto fromY = NarrowCast<int16_t>(std::min(lhs.y - 1, rhs.y - 1));
//     const auto toX = NarrowCast<int16_t>(std::max(lhs.x + 1, rhs.x + 1));
//     const auto toY = NarrowCast<int16_t>(std::max(lhs.y + 1, rhs.y + 1));
//
//     std::array<CellPosition, 6> testPositions;
//     uint8_t testPositionSize { 0 };
//
//     for (auto y = fromY; y <= toY; ++y) {
//         for (auto x = fromX; x <= toX; ++x) {
//             const CellPosition position { x, y };
//
//             if (position == lhs || position == rhs) {
//                 continue;
//             }
//             if (!_positionSystem.IsValidPosition(position)) {
//                 continue;
//             }
//             const bool isNeighbour = _positionSystem.IsNeighbourFor(lhs, position) || _positionSystem.IsNeighbourFor(rhs, position);
//             ASSERT(isNeighbour);
//             // if (!isNeighbour) {
//             //     continue;
//             // }
//
//             ASSERT(testPositionSize < testPositions.size());
//             testPositions[testPositionSize] = position;
//             testPositionSize += 1;
//         }
//     }
//
//     ASSERT(testPositionSize >= 1);
//     //
//     // std::uniform_int_distribution<uint16_t> offsetDistribution { 0, static_cast<uint16_t>(testPositionSize - 1) };
//     // const uint8_t offset = static_cast<uint8_t>(offsetDistribution(common::GetRandomEngine()));
//     //
//     // for (uint8_t i = 0; i < testPositionSize; ++i) {
//     //     const uint8_t index = (i + offset) % testPositionSize;
//     //     const CellPosition position = testPositions[index];
//     //     const EcsEntity id = _positionSystem.Find(position);
//     //     const bool emptyPosition = id == InvalidEcsEntity;
//     //     if (emptyPosition) {
//     //         return position;
//     //     }
//     // }
//
//     return InvalidCellPosition;
// }
//
// CellBrain ReproductionProcedure::MakeChildBrain(EcsEntity lhs, EcsEntity rhs) const
// {
//     // constexpr uint8_t crossoverPoint = 10;
//     // CrossoverAlgorithm crossover { crossoverPoint };
//     // return crossover.Combine(_brainSystem.Get(lhs), _brainSystem.Get(rhs));
//     return {};
// }