#pragma once

namespace common {

template <class T>
    requires std::derived_from<T, RegistrableSystem>
T& Registrar::Register(std::unique_ptr<T> system)
{
    std::unique_ptr<RegistrableSystem> rawSystem { system.release() };
    RegistrableSystem& reference = Register(std::move(rawSystem));
    return static_cast<T&>(reference);
}

}
