#include "age_system.h"

#include "components/graveyard_tag.h"

AgeSystem::AgeSystem(EcsWorld& ecsWorld)
    : SimulationEcsSystem(ecsWorld)
{
}
void AgeSystem::DoProcessComponents(CellId id, CellAge& age)
{
    age.value += 1;

    constexpr static uint16_t LimitAge = 100;
    if (age.value >= LimitAge) {
        AccessEcsWorld().emplace_or_replace<GraveyardTag>(id);
    }
}