#pragma once
#include "simulation_ecs_config.h"

#include <entt/entity/registry.hpp>

#include "simulation/simulation_system.h"
#include "simulation_component_type.h"

template <typename Derived, SimulationComponentType... Components>
class SimulationEcsSystem : public SimulationSystem {
public:
    explicit SimulationEcsSystem(EcsWorld& world)
        : _ecsWorld(&world)
    {
    }

    void DoSystemUpdate() final
    {
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