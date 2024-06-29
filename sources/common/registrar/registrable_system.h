#pragma once

#include "application_storage.h"

namespace common {

class RegistrableSystem {
public:
    virtual ~RegistrableSystem() = default;
    virtual std::error_code InitializeSystem(ApplicationStorage& storage) = 0;
    virtual void TerminateSystem() = 0;
};

}