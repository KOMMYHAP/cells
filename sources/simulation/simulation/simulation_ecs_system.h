#pragma once
#include "simulation_ecs_config.h"

#include "simulation/simulation_system.h"
#include "simulation_component_type.h"
#include "systems_ecs/graveyarded_cell_exclude.h"

template <class T>
concept SimulationEcsSystemHasExcludedComponents = requires(T t) {
    T::ExcludedComponents;
};

template <typename Derived, SimulationComponentType... Components>
class SimulationEcsSystem : public SimulationSystem {
public:
    explicit SimulationEcsSystem(EcsWorld& world)
        : _ecsWorld(&world)
    {
    }

    void DoSystemUpdate() final
    {
        constexpr auto ExcludedComponents = GetExcludedComponents();
        _ecsWorld->view<Components...>(ExcludedComponents).each([this]<typename... T0>(const EcsEntity& id, T0&&... components) noexcept {
            Self().DoProcessComponents(id, std::forward<T0>(components)...);
        });
    }

    const EcsWorld& GetEcsWorld() const { return *_ecsWorld; }
    EcsWorld& AccessEcsWorld() { return *_ecsWorld; }

private:
    static consteval auto GetExcludedComponents() noexcept
    {
        if constexpr (SimulationEcsSystemHasExcludedComponents<Derived>) {
            using Derived::ExcludedComponents;
        } else {
            return entt::exclude_t {};
        }
    }

    Derived& Self() { return static_cast<Derived&>(*this); }

    gsl::not_null<EcsWorld*> _ecsWorld;
};