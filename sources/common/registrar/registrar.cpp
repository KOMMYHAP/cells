#include "registrar.h"
#include "asserts/assert.h"
#include "storage/stack_storage.h"

namespace common {

std::error_code Registrar::RunInit()
{
    ASSERT(_state == State::Registration, "Registration phase was already completed!");
    _state = State::Initialized;

    for (const auto& system : _systems) {
        if (const std::error_code error = system->InitializeSystem(_storage)) {
            _state = State::FailedToInitialize;
            return error;
        }
    }
    return {};
}

void Registrar::RunTerm()
{
    ASSERT(_state == State::Initialized, "Initialization phase must be completed!");
    _state = State::Terminated;

    for (const auto& systemPtr : _systems | std::views::reverse) {
        systemPtr->TerminateSystem();
    }
    _systems.clear();
    _storage = {};
}

}