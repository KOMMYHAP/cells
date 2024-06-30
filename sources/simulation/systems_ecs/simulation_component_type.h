#pragma once

template <typename T>
concept SimulationComponentType = std::is_trivial_v<T>; ///< Component should be trivial for optimal performance
