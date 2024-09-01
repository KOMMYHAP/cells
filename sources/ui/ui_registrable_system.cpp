#include "ui_registrable_system.h"

#include "system/ui_system.h"

UiRegistrableSystem::UiRegistrableSystem() = default;
UiRegistrableSystem::~UiRegistrableSystem() = default;

std::error_code UiRegistrableSystem::InitializeSystem(ApplicationStorage& storage)
{
    _uiSystem = std::make_unique<UiSystem>(storage);
    return {};
}

void UiRegistrableSystem::TerminateSystem()
{
    _uiSystem.reset();
}

UiApplicationInterface* UiRegistrableSystem::GetUiApplicationInterface() const
{
    return _uiSystem.get();
}