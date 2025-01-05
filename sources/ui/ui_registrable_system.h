#pragma once

#include "registrar/registrable_system.h"
#include "ui_application_interface.h"

class UiSystem;

class UiRegistrableSystem final : public common::RegistrableSystem {
public:
    UiRegistrableSystem();
    ~UiRegistrableSystem() override;

    std::error_code InitializeSystem(ApplicationStorage& storage) override;
    void TerminateSystem() override;

    UiApplicationInterface* GetUiApplicationInterface() const;
    const UiSystem& GetUiSystem() const { return *_uiSystem; }
    UiSystem& ModifyUiSystem() { return *_uiSystem; }

private:
    UiSystem* _uiSystem { nullptr };
};