// #pragma once
//
// #include "base_script.h"
// #include "cell_factories/cell_factory_interface.h"
// #include "cell_factories/patrol_cell_factory.h"
// #include "cell_factories/random_cell_factory.h"
// #include "components/cell_age.h"
// #include "simulation_parameters.h"
//
// struct SimulationStatistics {
//     uint32_t cellsCount { 0 };
//     uint32_t generation { 0 };
//     uint64_t tick { 0 };
// };
//
// class SimulationScript final : public BaseScript {
// public:
//     SimulationScript(const common::StackStorage& systems, std::map<SpawnPolicy, ICellFactory*> factories);
//
//     std::error_code Perform() override;
//
//     void SetParameters(const SimulationParameters& parameters);
//     const SimulationStatistics& GetStatistics() const { return _stats; }
//
// private:
//     entt::registry
//     const common::StackStorage& _systems;
//     std::map<SpawnPolicy, ICellFactory*> _factories;
//     SimulationParameters _parameters;
//     SimulationStatistics _stats;
//     std::unique_ptr<PatrolCellFactory> _patrolCellFactory;
//     std::unique_ptr<RandomCellFactory> _randomCellFactory;
//     ICellFactory* _cellFactory { nullptr };
// };