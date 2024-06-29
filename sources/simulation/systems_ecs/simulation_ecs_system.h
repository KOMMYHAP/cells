#pragma once
#include "simulation_ecs_config.h"

#include <entt/entity/registry.hpp>

#include "simulation/simulation_system.h"

template <class Derived, class... Components>
class SimulationEcsSystem : public SimulationSystem {
public:
    explicit SimulationEcsSystem(EcsWorld& world)
        : _ecsWorld(&world)
    {
    }

    void DoSystemUpdate() final
    {
        _ecsWorld->view<Components...>().each([this](const CellId id, Components&&... components) {
            DowncastToImpl().DoProcessComponents(id, std::forward<Components>(components)...);
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