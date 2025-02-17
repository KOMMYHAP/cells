#pragma once

#include "components/generated/auto_graveyard_tag.h"
#include "simulation/simulation_ecs_config.h"

inline constexpr auto ExcludeGraveyardedCells = entt::exclude_t<GraveyardTag> {};