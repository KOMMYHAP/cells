#include "ui_registrable_system.h"

#include "system/ui_system.h"
#include "ui_config.h"

UiRegistrableSystem::UiRegistrableSystem() = default;
UiRegistrableSystem::~UiRegistrableSystem() = default;

std::error_code UiRegistrableSystem::InitializeSystem(ApplicationStorage& storage)
{
    const auto& config = storage.Get<UiConfig>();
    World& world = storage.Modify<World>();
    _uiSystem = &storage.Store<UiSystem>(world, config);
    return {};
}

void UiRegistrableSystem::TerminateSystem()
{
    _uiSystem = nullptr;
}

UiApplicationInterface* UiRegistrableSystem::GetUiApplicationInterface() const
{
    return _uiSystem;
}