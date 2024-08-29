#include "simulation/simulation_ecs_procedure.h"

namespace {
struct TestComponent {
    int value;
};

class TestEcsProcedure final : public EcsProcedureProxy<TestEcsProcedure, TestComponent> {
public:
    TestEcsProcedure(EcsWorld& world, SimulationVirtualMachine& vm)
        : _world(&world)
        , _vm(&vm)
    {
    }

    EcsWorld& AccessEcsWorld() { return *_world; }
    SimulationVirtualMachine& AccessVirtualMachine() { return *_vm; }

    ExecutionStatus ExecuteProcedure(CellId id, ProcedureContext& context, CellBrain& brain, TestComponent& component)
    {
        return ExecutionStatus::Success;
    }

private:
    gsl::not_null<EcsWorld*> _world;
    gsl::not_null<SimulationVirtualMachine*> _vm;
};
}

TEST(TestEcsProcedureTests, CompileCheck)
{
    static_assert(std::is_invocable_r_v<void, decltype(&ProcedureBase::Execute), TestEcsProcedure, ProcedureContext&>);
}