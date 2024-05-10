///*
//#include "world.h"
//
//#include "procedures/consume_procedure.h"
//#include "procedures/look_procedure.h"
//#include "procedures/move_procedure.h"
//#include "procedures/reproduction_procedure.h"
//
//World::World(common::Storage systems)
//    : _systems(std::move(systems))
//    , _simulation(_systems)
//{
//}
//
//// WorldRender::Config World::MakeRenderConfig(uint32_t cellSize, std::unique_ptr<sf::Shader> shader)
////{
////     ASSERT(cellSize < 255);
////     const sf::Color gray { 0xCCCCCCFF };
////     return {
////         std::move(shader),
////         { sf::Color::Magenta, sf::Color::Green, sf::Color::Black, gray },
////         static_cast<uint8_t>(cellSize)
////     };
//// }
//
//// SimulationVirtualMachine::Config World::MakeSimulationVmConfig(World* world)
////{
////     auto watcher = [world](ProcessorState state) {
////         if (state == ProcessorState::Good) {
////             return;
////         }
////
////         // Cell's brain has illegal instruction, make insult as punishment
////         const CellId id = world->GetSystems().Get<SimulationVirtualMachine>().GetRunningCellId();
////         world->ModifySystems().Modify<HealthSystem>().Set(id, CellHealth::Zero);
////     };
////     constexpr uint8_t systemInstructionPerStep = 8;
////     return {
////         world->ModifySystems(),
////         systemInstructionPerStep,
////         std::move(watcher)
////     };
//// }
////
//// SpawnSystem::Config World::MakeSpawnSystemConfig(float fullnessPercent)
////{
////     const auto targetPopulationSize = static_cast<uint32_t>(round(fullnessPercent * static_cast<float>(_idSystem.GetCellsCountLimit()) / 100.0f));
////     return {
////         _positionSystem,
////         _idSystem,
////         _spawner,
////         targetPopulationSize,
////     };
//// }
//
//void World::Update(sf::Time elapsedTime)
//{
//    _simulation.Run(elapsedTime);
//}
//
//const common::Storage& World::GetSystems() const
//{
//    return _parameters.Get<WorldParameters>().systems;
//}
//
//common::Storage& World::ModifySystems()
//{
//    return _parameters.Modify<WorldParameters>().systems;
//}
//*/
