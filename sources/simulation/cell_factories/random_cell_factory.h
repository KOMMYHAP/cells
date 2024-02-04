#pragma once

#include "cell_factory_interface.h"
#include "components/cell_id.h"

class ProcessorMemory;

class RandomCellFactory : public ICellFactory {
public:
    RandomCellFactory(std::optional<uint16_t> limitBrainSize = std::nullopt);

    Result Make() override;

private:
    bool InitMemory(ProcessorMemory& memory);

    uint16_t _brainSize { 0 };
};
