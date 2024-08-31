#pragma once

namespace common {

template <class T, class... Args>
    requires std::is_base_of_v<RegistrableSystem, T> && std::is_constructible_v<T, Args...>
T& Registrar::Register(Args&&... args)
{
    auto system = std::make_unique<T>(std::forward<Args>(args)...);
    auto& systemRef = *system;
    _systems.emplace_back(std::move(system));
    return systemRef;
}

}
