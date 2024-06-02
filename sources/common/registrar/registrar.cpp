#include "registrar.h"
#include "storage/stack_storage.h"

namespace common {

RegistrableSystem& Registrar::Register(std::unique_ptr<RegistrableSystem> system)
{
    Expects(_state == State::Registration, "Registration phase required!", _state);
    RegistrableSystem* rawPointer = system.get();
    _systems.push_back(std::move(system));
    return *rawPointer;
}

std::error_code Registrar::RunInit()
{
    Expects(_state == State::Registration, "Wrong registration phase!", _state);
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
    Expects(_state == State::Initialized, "Wrong registration phase!", _state);
    _state = State::Terminated;

    for (auto& system : std::ranges::reverse_view(_systems)) {
        system->TerminateSystem();
    }
    _systems.clear();
    _storage = {};
}

}