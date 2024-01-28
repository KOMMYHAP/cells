#pragma once

class ProcessorContext;

namespace processor::instructions {

class AddRegistryRegistry {
public:
    inline static constexpr uint8_t stackInCount = 0;
    inline static constexpr uint8_t stackOutCount = 0;
    inline static constexpr uint8_t registerInCount = 2;
    inline static constexpr uint8_t registerOutCount = 1;

    static bool Execute(ProcessorContext& context);
};

}