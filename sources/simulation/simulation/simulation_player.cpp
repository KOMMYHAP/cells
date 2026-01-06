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
        return timeToSpend >= timePerGeneration;
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
        timeToSpend -= timePerGeneration;
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
}

void SimulationPlayer::SetFixedSpeedMode(double generationsPerSecond)
{
    ASSERT(generationsPerSecond > 0.0, "Positive amount of generations allowed only!");
    mode = Mode::FixedSpeed;
    timeToSpend = {};
    timePerGeneration = Common::Time::FromSeconds(1.0 / generationsPerSecond);
}

void SimulationPlayer::UpdateElapsedTime(Common::Time elapsedTime)
{
    ASSERT(mode == Mode::FixedSpeed, "Mode 'FixedSpeed' required!");
    timeToSpend += elapsedTime;

    if (timeToSpend > Common::Time::FromSeconds(1.0)) {
        timeToSpend = Common::Time::FromSeconds(1.0);
    }
}
