#pragma once

#include "registrar/registrable_system.h"
#include "ui_application_interface.h"

class UiSystem;

class UiRegistrableSystem final : public common::RegistrableSystem {
public:
    UiRegistrableSystem();
    UiRegistrableSystem(const UiRegistrableSystem& other) = delete;
    UiRegistrableSystem(UiRegistrableSystem&& other) noexcept = delete;
    UiRegistrableSystem& operator=(const UiRegistrableSystem& other) = delete;
    UiRegistrableSystem& operator=(UiRegistrableSystem&& other) noexcept = delete;
    ~UiRegistrableSystem() override;

    std::error_code InitializeSystem(ApplicationStorage& storage) override;
    void TerminateSystem() override;

    UiApplicationInterface* GetUiApplicationInterface() const;

private:
    UiSystem* _uiSystem{nullptr};
};