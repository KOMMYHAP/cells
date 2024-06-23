#pragma once
#include "components/cell_id.h"
#include "system_handle.h"

class SystemBase;
class SystemRegistry;

class SystemExecutor {
public:
    SystemExecutor(SystemRegistry& registry, std::span<SystemHandle> handles);
    
    void Process();

private:
    std::vector<CellId> _cells;
    std::vector<SystemBase*> _systems;
};
