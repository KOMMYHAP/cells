#pragma once
#include "time/time.h"


class SimulationPlayer {
public:
    enum class Mode {
        ManualStep,
        FixedSpeed
    };

    Mode GetCurrentMode();

    bool ShouldPlayFrame() const;
    void PlayFrame();

public: //< ManualMode
    void Pause();
    void RequestStep();

public: //< FixedSpeedMode
    void Resume();
    void SetFixedSpeedMode(double generationsPerSecond);
    void UpdateElapsedTime(Common::Time elapsedTime);

private:
    Mode mode { Mode::ManualStep };
    bool stepRequested { false };
    Common::Time timeToSpend;
    Common::Time timePerGeneration;
};
