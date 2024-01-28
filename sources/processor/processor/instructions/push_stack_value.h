#pragma once

class ProcessorContext;

namespace processor::instructions {

class PushStackValue {
public:
    inline static constexpr uint8_t stackInCount = 0;
    inline static constexpr uint8_t stackOutCount = 0;
    inline static constexpr uint8_t registerInCount = 0;
    inline static constexpr uint8_t registerOutCount = 0;

    static bool Execute(ProcessorContext& context);
};

}