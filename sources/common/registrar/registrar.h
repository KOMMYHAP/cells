#pragma once

#include "registrable_system.h"
#include "storage/stack_storage.h"

namespace common {

class Registrar {
public:
    template <class T>
        requires std::derived_from<T, RegistrableSystem>
    T& Register(std::unique_ptr<T> system);

    RegistrableSystem& Register(std::unique_ptr<RegistrableSystem> system);

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
