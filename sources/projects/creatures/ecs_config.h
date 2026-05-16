#pragma once

#pragma once

#include "asserts/assert.h"

#define ENTT_NOEXCEPTION

#if defined(COMMON_ASSERTS_ENABLED)
#define ENTT_ASSERT(expression, message) ASSERT(expression, message)
#else
#define ENTT_DISABLE_ASSERT
#endif

#define ENTT_STANDARD_CPP

#include <entt/config/config.h>
#include <entt/entt.hpp>

using EcsWorld = entt::registry;
using EcsEntity = EcsWorld::entity_type;
inline constexpr EcsEntity InvalidEcsEntity { entt::null };