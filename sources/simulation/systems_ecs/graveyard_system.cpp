#include "graveyard_system.h"

#include "components/graveyard_tag.h"

GraveyardSystem::GraveyardSystem(EcsWorld& world)
    : _world(&world)
{
}
void GraveyardSystem::DoSystemUpdate()
{
    const auto graveyard = _world->group<GraveyardTag>();
    _world->destroy(graveyard.begin(), graveyard.end());
}