#include "simulation_player.h"
#include "asserts/assert.h"

SimulationPlayer::Mode SimulationPlayer::GetCurrentMode()
{
    return mode;
}

bool SimulationPlayer::ShouldPlayFrame() const
{
    if (mode == Mode::ManualStep) {
        return stepRequested;
    }
    if (mode == Mode::FixedSpeed) {
        return timeToSpend >= requiredTimePerStep;
    }

    ASSERT(mode == Mode::ManualStep, "Unknown mode!");
    return false;
}

void SimulationPlayer::PlayFrame()
{
    ASSERT(ShouldPlayFrame(), "Cannot play frame now!");

    if (mode == Mode::ManualStep) {
        stepRequested = false;
    }
    if (mode == Mode::FixedSpeed) {
        timeToSpend -= requiredTimePerStep;
    }
}

void SimulationPlayer::Pause()
{
    mode = Mode::ManualStep;
    stepRequested = false;
}

void SimulationPlayer::RequestStep()
{
    ASSERT(mode == Mode::ManualStep, "Mode 'ManualStep' required!");
    stepRequested = true;
}

void SimulationPlayer::Resume()
{
    mode = Mode::FixedSpeed;
    timeToSpend = {};
    stepRequested = true;
}

void SimulationPlayer::SetFixedSpeedMode(double stepsPerSecond)
{
    ASSERT(stepsPerSecond > 0.0, "Positive amount of generations allowed only!");
    mode = Mode::FixedSpeed;
    timeToSpend = {};
    requiredTimePerStep = Common::Time::FromSeconds(1.0 / stepsPerSecond);
}

void SimulationPlayer::UpdateElapsedTime(Common::Time elapsedTime, Common::Time timePerStep)
{
    ASSERT(mode == Mode::FixedSpeed, "Mode 'FixedSpeed' required!");
    timeToSpend += elapsedTime;
    elapsedTimePerStep = timePerStep;
    requiredTimePerStep = std::max(elapsedTimePerStep, requiredTimePerStep);

    if (timeToSpend > Common::Time::FromSeconds(1.0)) {
        timeToSpend = Common::Time::FromSeconds(1.0);
    }
}
