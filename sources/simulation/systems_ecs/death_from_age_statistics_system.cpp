#include "death_from_age_statistics_system.h"

#include "components/death_from_age_tag.h"

DeathFromAgeStatisticsSystem::DeathFromAgeStatisticsSystem(EcsWorld& world, SimulationStatisticsProvider& stats)
    : _world(&world)
    , _stats(&stats)
{
}
void DeathFromAgeStatisticsSystem::DoSystemUpdate()
{
    const size_t count = _world->view<const DeathFromAgeTag>().size();
    _stats->SetDeathFromAge(count);
}