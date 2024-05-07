#pragma once

#include "base_script.h"

namespace common {
class Storage;
}

class TickScript final : public BaseScript {
public:
    TickScript(const common::Storage & systems);

    void Perform() override;

private:
    struct Parameters;
    std::unique_ptr<Parameters> _parameters;
    const common::Storage& _systems;
};