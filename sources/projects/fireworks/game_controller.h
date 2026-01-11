#pragma once
#include "components/generated/auto_cell_position.h"
#include "random/random.h"
#include "time/time.h"

class GameController {
public:
    Common::Time GetFrameTime() const { return _frameTime; }
    void SetFrameTime(Common::Time time) { _frameTime = time; }

    CellPosition GetGravityCenter() const { return _gravityCenter; }
    void SetGravityCenter(CellPosition gravityCenter) { _gravityCenter = gravityCenter; }

    float GetGravityConstant() const { return _gravityConstant; }
    void SetGravityConstant(float gravityConstant) { _gravityConstant = gravityConstant; }

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
    CellPosition _gravityCenter {};
    float _gravityConstant { 0.0f };
    bool _gravityEnabled { false };
    int32_t _emittersCount { 0 };
    int32_t _particlesCount { 0 };
};