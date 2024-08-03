#pragma once

#include "asserts/assert.h"

#define ENTT_NOEXCEPTION
#define ENTT_USE_ATOMIC
#define ENTT_ASSERT(expression, message) ASSERT(expression, message)

#ifdef ENTT_CONFIG_CONFIG_H
#error "You should include simulation_ecs_config.h before any include from entt/*!"
#endif

#include "entt/config/config.h"
#include "entt/fwd.hpp"

#include "components/cell_id.h"

using EcsWorld = entt::basic_registry<CellId>;