#include "components/generated/auto_has_life_tag.h"
#include "components/generated/auto_will_born_tag.h"
#include "generated/auto_born_new_life_system.h"

void BornNewLifeSystem::DoProcessComponents(EcsEntity id)
{
    _ecsWorld->remove<WillBornTag>(id);
    _ecsWorld->emplace<HasLifeTag>(id);
}