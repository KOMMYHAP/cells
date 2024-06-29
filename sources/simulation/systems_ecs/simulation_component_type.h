#pragma once

template <typename T>
concept SimulationComponentType = true
    && std::is_trivial_v<T> ///< Component should be trivial for optimal performance
    && !std::is_empty_v<T>; ///< "ECS framework 'entt' does not support empty type in view. Type can contain a dummy field as a workaround!"
