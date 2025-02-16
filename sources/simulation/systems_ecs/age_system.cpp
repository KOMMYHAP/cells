#include "age_system.h"

#include "components/cell_age.h"
#include "components/death_from_age_tag.h"
#include "components/graveyard_tag.h"
#include "simulation/simulation_ecs_system.h"

namespace {

class AgeSystem final : public SimulationEcsSystem<AgeSystem, CellAge> {
public:
    explicit AgeSystem(EcsWorld& ecsWorld);

    void DoProcessComponents(EcsEntity id, CellAge& age);
};

AgeSystem::AgeSystem(EcsWorld& ecsWorld)
    : SimulationEcsSystem(ecsWorld)
{
}
void AgeSystem::DoProcessComponents(EcsEntity id, CellAge& age)
{
    age.value += 1;

    constexpr static uint16_t LimitAge = 2'000;
    if (age.value >= LimitAge) {
        EcsWorld& world = AccessEcsWorld();
        world.emplace<DeathFromAgeTag>(id);
        world.emplace_or_replace<GraveyardTag>(id);
    }
}

}

std::unique_ptr<SimulationSystem> MakeAgeSystem(const SimulationStorage& storage)
{
    EcsWorld& world = storage.Modify<EcsWorld>();
    return std::make_unique<AgeSystem>(world);
}