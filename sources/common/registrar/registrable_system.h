#pragma once

namespace common {

class StackStorage;

class RegistrableSystem {
public:
    virtual ~RegistrableSystem() = default;
    virtual std::error_code InitializeSystem(StackStorage& storage) = 0;
    virtual void TerminateSystem() = 0;
};

}