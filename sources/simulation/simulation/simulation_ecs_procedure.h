#pragma once

#include "components/cell_brain.h"
#include "components/deferred_procedure_execution.h"
#include "procedures/procedure.h"
#include "simulation/simulation_procedure_context.h"
#include "simulation/simulation_ecs_system.h"

namespace Details {

template <class T>
struct ProcedureTag {
    using ProcedureType = T;
};

template <class ProcedureImpl>
class DeferredProcedureProxy : public ProcedureBase {
public:
    void Execute(ProcedureContext& context) final;

private:
    ProcedureImpl& CastToImpl();
};
}

template <class EcsProcedureImpl, SimulationComponentType... Components>
class EcsProcedureProxy : public SimulationSystem, public Details::DeferredProcedureProxy<EcsProcedureImpl> {
public:
    enum class ExecutionStatus {
        Success,
        Error
    };

    void DoSystemUpdate() final;

private:
    template <class... FilteredComponents>
    void DoProcessComponents(EcsEntity id, CellBrain& brain, DeferredProcedureExecution& deferredExecution, FilteredComponents&&... components);

    EcsProcedureImpl& CastToImpl();
};

#include "simulation_ecs_procedure.hpp"