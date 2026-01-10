#pragma once
#include "components/generated/auto_cell_position.h"
#include "time/time.h"

class GameController {
public:
    float GetFramesPerSecond() const;
    Common::Time GetFrameTime() const;
    CellPosition GetGravityCenter() const;
    float GetGravityConstant() const;

    void SetEmittersCount(int32_t count);
    int32_t GetEmittersCount() const;

    void SetParticlesCount(int32_t count);
    int32_t GetParticlesCount() const;

private:
};