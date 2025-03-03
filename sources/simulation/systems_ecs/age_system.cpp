#include "generated/auto_age_system.h"

#include "components/generated/auto_cell_age.h"
#include "components/generated/auto_death_from_age_tag.h"
#include "components/generated/auto_graveyard_tag.h"

void AgeSystem::DoProcessComponents(EcsEntity id, CellAge& cellAge)
{
    cellAge.value += 1;

    constexpr static uint16_t LimitAge = 2'000;
    if (cellAge.value >= LimitAge) {
        EcsWorld& world = *_ecsWorld;
        world.emplace<DeathFromAgeTag>(id);
        world.emplace_or_replace<GraveyardTag>(id);
    }
}