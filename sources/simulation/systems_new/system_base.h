#pragma once

#include "system_context.h"

#include "components/cell_id.h"
#include "components/component_handle.h"
#include "components/component_registry.h"
#include "components/component_storage.h"

class SystemBase {
public:
    SystemBase(ComponentRegistry& registry, std::span<const ComponentHandle> handles);
    virtual ~SystemBase() = default;

    SystemBase(const SystemBase&) = delete;
    const SystemBase& operator=(const SystemBase&) = delete;
    SystemBase(SystemBase&&) = delete;
    const SystemBase& operator=(SystemBase&&) = delete;

    // todo: method should be hidden from public interface
    // See more by link: https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Friendship_and_the_Attorney-Client
    void ExecutorProcess(std::span<const CellId> cells);

protected:
    virtual void DoProcess(const SystemContext& context) = 0;

private:
    struct ComponentEntry {
        ComponentHandle handle;
        gsl::not_null<ComponentStorage*> storage; 
        uint16_t sizeInBytes { 0 };
        std::byte* startAddress { nullptr };
    };

    uint32_t _cellsCount { 0 };
    std::span<const ComponentHandle> handles;
    std::vector<ComponentEntry> _components;
    std::vector<std::byte*> _componentBuffer;
};
