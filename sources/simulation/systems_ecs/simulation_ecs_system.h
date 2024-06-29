#pragma once
#include "components/cell_position.h"
#include "components/spawn_place.h"
#include "simulation_ecs_config.h"

#include <entt/entity/registry.hpp>

#include "simulation/simulation_system.h"

template <typename Derived, typename... Components>
class SimulationEcsSystem : public SimulationSystem {
public:
    explicit SimulationEcsSystem(EcsWorld& world)
        : _ecsWorld(&world)
    {
    }

    void DoSystemUpdate() final
    {
        static_assert((!std::is_empty_v<Components> && ...), "ECS framework 'entt' does not support empty type in view. Type can contain a dummy field as a workaround!");
        _ecsWorld->view<Components...>().each([this]<typename... T0>(const CellId& id, T0&&... components) {
            DowncastToImpl().DoProcessComponents(id, std::forward<T0>(components)...);
        });
    }

    const EcsWorld& GetEcsWorld() const { return *_ecsWorld; }
    EcsWorld& AccessEcsWorld() { return *_ecsWorld; }

private:
    Derived& DowncastToImpl()
    {
        return static_cast<Derived&>(*this);
    }

    gsl::not_null<EcsWorld*> _ecsWorld;
};