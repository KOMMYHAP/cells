#include "move_procedure.h"

#include "simulation_procedure_context.h"
#include "systems/position_system.h"
#include "systems/simulation_virtual_machine.h"

#include <entt/entity/registry.hpp>

namespace {
bool TryExtractMoveDirection(uint8_t data, MoveDirection& direction)
{
    switch (data) {
    case static_cast<uint8_t>(MoveDirection::Right):
        direction = MoveDirection::Right;
        return true;
    case static_cast<uint8_t>(MoveDirection::Left):
        direction = MoveDirection::Left;
        return true;
    case static_cast<uint8_t>(MoveDirection::Down):
        direction = MoveDirection::Down;
        return true;
    case static_cast<uint8_t>(MoveDirection::Up):
        direction = MoveDirection::Up;
        return true;
    default:
        return false;
    }
}
}

MoveProcedure::MoveProcedure(EcsWorld& world)
    : _world(&world)
{
}

void MoveProcedure::Execute(ProcedureContext& context)
{
    const auto [readArgs, rawDirection] = context.TryPopArgs<uint8_t>();
    if (!readArgs) {
        return;
    }
    MoveDirection direction;
    if (!TryExtractMoveDirection(rawDirection, direction)) {
        context.MarkProcedureAsInvalid();
        return;
    }

    const CellId* id = context.GetExternalContext().Get<SimulationProcedureContext>().id;
    _world->emplace_or_replace<MoveDirection>(*id, direction);
}