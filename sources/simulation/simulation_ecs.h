#pragma once

#include "components/cell_id.h"

namespace entt {
template <typename Entity, typename = std::allocator<Entity>>
class basic_registry;
}

using EcsWorld = entt::basic_registry<CellId>;