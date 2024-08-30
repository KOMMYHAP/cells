#pragma once

#include "asserts/assert.h"

#define ENTT_NOEXCEPTION
#define ENTT_ASSERT(expression, message) ASSERT(expression, message)

#ifdef ENTT_CONFIG_CONFIG_H
#error "You should include simulation_ecs_config.h before any include from entt/*!"
#endif

#include <entt/config/config.h>
#include <entt/entt.hpp>

using EcsWorld = entt::registry;
using EcsEntity = EcsWorld::entity_type;
inline constexpr EcsEntity InvalidEcsEntity { entt::null };