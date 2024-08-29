﻿#pragma once

#include "simulation/simulation_virtual_machine.h"
#include "simulation_ecs_procedure.h"

namespace Details {

template <class ProcedureImpl>
void DeferredProcedureProxy<ProcedureImpl>::Execute(ProcedureContext& context)
{
    const CellId id = context.GetUserData().Get<SimulationProcedureContext>().id;
    ProcedureImpl& impl = CastToImpl();
    EcsWorld& world = impl.AccessEcsWorld();
    world.emplace<ProcedureImplTag<ProcedureImpl>>(id);
    world.emplace<DeferredProcedureExecution>(id, context);
}

template <class ProcedureImpl>
ProcedureImpl& DeferredProcedureProxy<ProcedureImpl>::CastToImpl()
{
    static_assert(
        std::is_invocable_r_v<EcsWorld&, decltype(&ProcedureImpl::AccessEcsWorld), ProcedureImpl>,
        "ProcedureImpl must have method EcsWorld& AccessEcsWorld();");
    return static_cast<ProcedureImpl&>(*this);
}

}

template <class EcsProcedureImpl, SimulationComponentType... Components>
void EcsProcedureProxy<EcsProcedureImpl, Components...>::DoSystemUpdate()
{
    CastToImpl()
        .AccessEcsWorld()
        .template view<CellBrain, DeferredProcedureExecution, Components...>()
        .each([this]<typename... T0>(const CellId& id, CellBrain& brain, DeferredProcedureExecution& deferredExecution, T0&&... components) noexcept {
            DoProcessComponents(id, brain, deferredExecution, std::forward<T0>(components)...);
        });
}

template <class EcsProcedureImpl, SimulationComponentType... Components>
template <class... FilteredComponents>
void EcsProcedureProxy<EcsProcedureImpl, Components...>::DoProcessComponents(CellId id, CellBrain& brain, DeferredProcedureExecution& deferredExecution, FilteredComponents&&... components)
{
    static_assert(
        std::is_invocable_r_v<ExecutionStatus, decltype(&EcsProcedureImpl::ExecuteProcedure), EcsProcedureImpl, CellId, ProcedureContext&, CellBrain&, FilteredComponents...>,
        "EcsProcedureImpl must have method ExecutionStatus ExecuteProcedure(CellId, ProcedureContext&, CellBrain&, FilteredComponents...);\n"
        "Where FilteredComponent is non-empty data structures");

    ProcedureContext& context = deferredExecution.context;
    const ExecutionStatus status = CastToImpl().ExecuteProcedure(id, context, brain, std::forward<FilteredComponents>(components)...);
    if (status == ExecutionStatus::Success) {
        CastToImpl().AccessVirtualMachine().CompletePendingProcedure(id, brain, context);
    } else if (status == ExecutionStatus::Error) {
        if (!context.IsFailed()) {
            context.AbortProcedure();
        }
    }
    CastToImpl().AccessEcsWorld().template remove<DeferredProcedureExecution, Details::ProcedureImplTag<EcsProcedureImpl>>(id);
}

template <class EcsProcedureImpl, SimulationComponentType... Components>
EcsProcedureImpl& EcsProcedureProxy<EcsProcedureImpl, Components...>::CastToImpl()
{
    static_assert(
        std::is_invocable_r_v<EcsWorld&, decltype(&EcsProcedureImpl::AccessEcsWorld), EcsProcedureImpl>,
        "EcsProcedureImpl must have method EcsWorld& AccessEcsWorld();");
    static_assert(
        std::is_invocable_r_v<SimulationVirtualMachine&, decltype(&EcsProcedureImpl::AccessVirtualMachine), EcsProcedureImpl>,
        "EcsProcedureImpl must have method SimulationVirtualMachine& AccessVirtualMachine();");

    return static_cast<EcsProcedureImpl&>(*this);
}