#include "world_statistics.h"

void WorldStatistics::AddFrame(Common::Time elapsedTime)
{
    _elapsedFramesCount += 1;
    _frameSamples.AddSample(elapsedTime);
}

Common::Time WorldStatistics::GetFrameTime() const
{
    return _frameSamples.CalcMedian();
}

void WorldStatistics::AddTick(Common::Time elapsedTime)
{
    _elapsedTicksCount += 1;
    _tickSamples.AddSample(elapsedTime);
}

Common::Time WorldStatistics::GetTickTime() const
{
    return _tickSamples.CalcMedian();
}