#pragma once
#include "components/generated/auto_cell_position.h"
#include "random/random.h"
#include "time/time.h"

struct GravityUnit {
    double positionX{0.0f};
    double positionY{0.0f};

    double constant{0.0};
    double mass{0.0};
};

class GameController {
public:
    Common::Time GetFrameTime() const { return _frameTime; }
    void SetFrameTime(Common::Time time) { _frameTime = time; }

    GravityUnit GetGravityUnit() const { return _gravityUnit; }
    GravityUnit& ModifyGravityUnit() { return _gravityUnit; }
    void SetGravityCenter(const GravityUnit & gravityUnit) { _gravityUnit = gravityUnit; }

    bool IsGravityEnabled() const { return _gravityEnabled; }
    void SetGravityEnabled(bool value) { _gravityEnabled = value; }

    void SetEmittersCount(int32_t count) { _emittersCount = count; }
    int32_t GetEmittersCount() const { return _emittersCount; }

    void SetParticlesCount(int32_t count) { _particlesCount = count; }
    int32_t GetParticlesCount() const { return _particlesCount; }

    Random::Accessor ModifyRandom() { return Random::Accessor { _randomEngine }; }

private:
    Random::Engine _randomEngine;
    Common::Time _frameTime;
    GravityUnit _gravityUnit;
    bool _gravityEnabled { false };
    int32_t _emittersCount { 0 };
    int32_t _particlesCount { 0 };
};