#include "registrar/registrar.h"

class RegistrarTestSystem : public common::RegistrableSystem {
public:
    std::error_code InitializeSystem(common::StackStorage& storage) override;
    void TerminateSystem() override;

    enum class State {
        JustCreated,
        Initialized,
        Terminated
    };
    State state { State::JustCreated };
    bool simulateError { false };
    std::function<void()> onInit;
    std::function<void()> onTerm;
};

std::error_code RegistrarTestSystem::InitializeSystem(common::StackStorage& storage)
{
    if (simulateError) {
        return std::make_error_code(std::errc::argument_out_of_domain);
    }
    if (onInit) {
        onInit();
    }
    state = State::Initialized;
    return std::error_code();
}

void RegistrarTestSystem::TerminateSystem()
{
    if (onTerm) {
        onTerm();
    }
    state = State::Terminated;
}

TEST(RegistrarTest, InitTermOneSystem)
{
    common::Registrar registrar;
    auto system = std::make_unique<RegistrarTestSystem>();
    auto* ptr = system.get();
    registrar.Register(std::move(system));
    ASSERT_EQ(ptr->state, RegistrarTestSystem::State::JustCreated);
    const std::error_code error = registrar.RunInit();
    ASSERT_FALSE(error);
    ASSERT_EQ(ptr->state, RegistrarTestSystem::State::Initialized);
    registrar.RunTerm();
}

TEST(RegistrarTest, AbortInitOnError)
{
    common::Registrar registrar;
    auto system = std::make_unique<RegistrarTestSystem>();
    system->simulateError = true;
    auto* errorSystem = system.get();
    registrar.Register(std::move(system));

    system = std::make_unique<RegistrarTestSystem>();
    auto* skippedSystem = system.get();
    registrar.Register(std::move(system));

    const std::error_code error = registrar.RunInit();
    ASSERT_TRUE(error);
    ASSERT_EQ(errorSystem->state, RegistrarTestSystem::State::JustCreated);
    ASSERT_EQ(skippedSystem->state, RegistrarTestSystem::State::JustCreated);
}

TEST(RegistrarTest, ReverseTerminationOrder)
{
    common::Registrar registrar;
    std::stack<RegistrarTestSystem*> systems;

    auto addSystem = [&]() {
        auto system = std::make_unique<RegistrarTestSystem>();
        system->onInit = [system = system.get(), &systems]() {
            systems.push(system);
        };
        system->onTerm = [system = system.get(), &systems]() {
            ASSERT_EQ(system, systems.top());
            systems.pop();
        };
        registrar.Register(std::move(system));
    };

    addSystem();
    addSystem();

    const std::error_code error = registrar.RunInit();
    ASSERT_FALSE(error);
    registrar.RunTerm();
}