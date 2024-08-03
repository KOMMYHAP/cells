#pragma once

template <class T>
concept MemoryType = !std::is_reference_v<T> && !std::is_pointer_v<T> && std::is_trivial_v<T>;