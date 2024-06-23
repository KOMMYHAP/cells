// #pragma once
//
// #include "systems/system_base.h"
//
// #include "components/component_handle.h"
// #include "components/component_registry.h"
//
// class ComponentStorage;
// class ComponentRegistry;
//
// class SequenceSystem final : public SystemBase {
// public:
//     SequenceSystem(ComponentRegistry& registry, std::string_view name, std::span<const ComponentHandle> handles);
//
//     void SetFunction(std::function<void(const SystemContext&)> func) { _function = std::move(func); }
//     void SetSequence(std::span<const CellId> ids) { _ids = ids; }
//
//     void Process() override;
//
// private:
//     std::span<const CellId> _ids;
//     std::function<void(const SystemContext&)> _function;
// };