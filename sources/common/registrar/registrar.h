#pragma once

#include "registrable_system.h"
#include "storage/stack_storage.h"

namespace common {

class Registrar {
public:
    template <class T, class... Args>
        requires std::is_base_of_v<RegistrableSystem, T> && std::is_constructible_v<T, Args...>
    T& Register(Args&&... args);

    std::error_code RunInit();
    void RunTerm();

private:
    enum class State {
        Registration,
        Initialized,
        FailedToInitialize,
        Terminated
    };

    State _state { State::Registration };
    StackStorage _storage;
    std::vector<std::unique_ptr<RegistrableSystem>> _systems;
};

}

#include "registrar.hpp"
