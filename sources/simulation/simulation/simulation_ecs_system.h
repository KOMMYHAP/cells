#pragma once
#include "simulation_ecs_config.h"
#include "components/cell_id.h"

#include "simulation/simulation_system.h"
#include "simulation_component_type.h"
#include "systems_ecs/graveyarded_cell_exclude.h"

template <typename Derived, SimulationComponentType... Components>
class SimulationEcsSystem : public SimulationSystem {
public:
    explicit SimulationEcsSystem(EcsWorld& world)
        : _ecsWorld(&world)
    {
    }

    void DoSystemUpdate() final
    {
        _ecsWorld->view<Components...>(ExcludeGraveyardedCells).each([this]<typename... T0>(const EcsEntity& id, T0&&... components) noexcept {
            static_cast<Derived&>(*this).DoProcessComponents(id, std::forward<T0>(components)...);
        });
    }

    const EcsWorld& GetEcsWorld() const { return *_ecsWorld; }
    EcsWorld& AccessEcsWorld() { return *_ecsWorld; }

private:
    gsl::not_null<EcsWorld*> _ecsWorld;
};