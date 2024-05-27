#pragma once

#include "base_script.h"
#include "cell_factories/cell_factory_interface.h"
#include "storage/stack_storage.h"

struct WorldParameters {
    uint32_t cellsCountLimit { 0 };
    common::StackStorage systems;
    std::vector<std::unique_ptr<ICellFactory>> factories;
    std::unique_ptr<BaseScript> simulationScript;
};