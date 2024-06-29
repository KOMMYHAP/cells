#include "move_procedure.h"

#include "simulation_procedure_context.h"
#include "systems_ecs/position_system.h"
#include "systems_ecs/simulation_virtual_machine.h"

#include <entt/entity/registry.hpp>

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
    if (!TryParseMoveDirection(rawDirection, direction)) {
        context.MarkProcedureAsInvalid();
        return;
    }

    const CellId* id = context.GetExternalContext().Get<SimulationProcedureContext>().id;
    _world->emplace_or_replace<MoveDirection>(*id, direction);
}