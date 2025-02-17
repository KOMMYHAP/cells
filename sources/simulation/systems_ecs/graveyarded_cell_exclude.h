#pragma once

#include "components/generated/graveyard_tag.h"
#include "simulation/simulation_ecs_config.h"

inline constexpr auto ExcludeGraveyardedCells = entt::exclude_t<GraveyardTag> {};