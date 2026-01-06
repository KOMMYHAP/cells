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
    void SetFixedSpeedMode(double stepsPerSeconds);
    void UpdateElapsedTime(Common::Time elapsedTime, Common::Time timePerStep);

private:
    Mode mode { Mode::ManualStep };
    bool stepRequested { false };
    Common::Time timeToSpend;
    Common::Time requiredTimePerStep;
    Common::Time elapsedTimePerStep;
};
