#include "age_system.h"

#include "components/death_from_age_tag.h"
#include "components/graveyard_tag.h"

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