//#pragma once
//
//#include "storage/storage.h"
//#include "updatable.h"
//#include "simulation.h"
//
//class World : public Updatable {
//public:
////    struct Config {
////        uint32_t width;
////        uint32_t height;
////        uint32_t cellSize;
////        uint8_t fullnessPercent;
////        std::unique_ptr<sf::Shader> shader;
////    };
////    World(Config&& config);
//    World(common::Storage systems);
//
//    void Update(sf::Time elapsedTime) override;
//
//    const common::Storage& GetSystems() const;
//    common::Storage& ModifySystems();
//
//private:
////    static WorldRender::Config MakeRenderConfig(uint32_t cellSize, std::unique_ptr<sf::Shader> shader);
////    SpawnSystem::Config MakeSpawnSystemConfig(float fullnessPercent);
////    static SimulationVirtualMachine::Config MakeSimulationVmConfig(World* world);
//
//    common::Storage _parameters;
//    Simulation _simulation;
////    Statistics _statistics;
//};