#pragma once

template <typename T>
concept SimulationComponentType = true;

template <typename T>
concept SimulationProcedureTag = std::is_empty_v<T>;
