#include "move_procedure.h"

#include "procedures/procedure_context.h"
#include "simulation/cell_locator.h"
#include "simulation/simulation_procedure_context.h"

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
    Direction direction;
    if (!TryParseDirection(rawDirection, direction)) {
        context.AbortProcedure();
        return;
    }

    const CellId id = context.GetUserData().Get<SimulationProcedureContext>().id;
    _world->emplace<MoveDirection>(id, direction);
}