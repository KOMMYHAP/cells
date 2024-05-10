#include "registrar.h"
#include "storage/stack_storage.h"

namespace common {

void Registrar::Register(std::unique_ptr<RegistrableSystem> system)
{
    ASSERT(_state == State::Registration, "Registration phase required!", _state);
    _systems.push_back(std::move(system));
}

std::error_code Registrar::RunInit()
{
    ASSERT(_state == State::Registration, "Wrong registration phase!", _state);
    _state = State::Initialized;

    for (auto& system : _systems) {
        const std::error_code error = system->InitializeSystem(_storage);
        if (error) {
            _state = State::FailedToInitialize;
            return error;
        }
    }
    return {};
}

void Registrar::RunTerm()
{
    ASSERT(_state == State::Initialized, "Wrong registration phase!", _state);
    _state = State::Terminated;

    for (auto& system : std::ranges::reverse_view(_systems)) {
        system->TerminateSystem();
    }
    _systems.clear();
    _storage = {};
}

}
