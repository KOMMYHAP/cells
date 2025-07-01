#pragma once
#include "sample_counter.h"
#include "time/time.h"

class WorldStatistics {
public:
    void AddFrame(Common::Time elapsedTime);
    int64_t GetElapsedFramesCount() const { return _elapsedFramesCount; }
    Common::Time GetFrameTime() const;

    void AddTick(Common::Time elapsedTime);
    int64_t GetElapsedTicksCount() const { return _elapsedTicksCount; }
    Common::Time GetTickTime() const;

private:
    static constexpr int32_t TOTAL_SAMPLES_COUNT { 30 };

    Common::SampleCounter<Common::Time, TOTAL_SAMPLES_COUNT> _frameSamples;
    int64_t _elapsedFramesCount { 0 };
    Common::SampleCounter<Common::Time, TOTAL_SAMPLES_COUNT> _tickSamples;
    int64_t _elapsedTicksCount { 0 };
};