#include "app_statistics.h"

void AppStatistics::AddFrame(Common::Time elapsedTime)
{
    _elapsedFramesCount += 1;
    _frameSamples.AddSample(elapsedTime);
}

Common::Time AppStatistics::GetMedianFrameTime() const
{
    return _frameSamples.CalcMedian();
}

Common::Time AppStatistics::GetAverageFrameTime() const
{
    return _frameSamples.CalcAverage();
}