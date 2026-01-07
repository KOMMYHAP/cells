#include "components/generated/auto_has_life_tag.h"
#include "components/generated/auto_should_collect_neighbour.h"
#include "components/generated/auto_will_die_tag.h"
#include "generated/auto_remove_died_cell_system.h"

void RemoveDiedCellSystem::DoProcessComponents(EcsEntity id)
{
    _ecsWorld->remove<
        WillDieTag,
        HasLifeTag>(id);
}