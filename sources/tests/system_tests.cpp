#include "simulation/simulation_ecs_config.h"
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

    ExecutionStatus ExecuteProcedure(EcsEntity id, ProcedureContext& context, CellBrain& brain, TestComponent& component)
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

TEST(StdRanges, Sample)
{
    static constexpr int TargetCellsCount = { 10 };
    static constexpr int SpawnPlaceToCreate = { 8 };
    std::array<int, TargetCellsCount> spawnPlacesBuffer;

    for (int i = 0; i < SpawnPlaceToCreate; ++i) {
        spawnPlacesBuffer[i] = i / 2;
    }

    auto spawnPlaces = spawnPlacesBuffer | std::views::take(SpawnPlaceToCreate);
    std::ranges::sort(spawnPlaces);
    const auto duplicates = std::ranges::unique(spawnPlaces);
    const auto uniqueSpawnPlaces = spawnPlaces | std::views::take(spawnPlaces.size() - duplicates.size());

    ASSERT_EQ(uniqueSpawnPlaces.size(), 4);
    ASSERT_EQ(uniqueSpawnPlaces[0], 0);
    ASSERT_EQ(uniqueSpawnPlaces[1], 1);
    ASSERT_EQ(uniqueSpawnPlaces[2], 2);
    ASSERT_EQ(uniqueSpawnPlaces[3], 3);
}