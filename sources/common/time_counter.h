#pragma once

namespace common {

class TimeCounter {
public:
    void AddSample(sf::Time elapsedTime);
    void Reset();

    bool IsEmpty() const { return _samplesCount == 0; }
    bool IsReady() const { return _samplesCount > 0; }
    bool IsFull() const { return _samplesCount == SamplesCountLimit; }

    sf::Time GetAverageTime() const;
    sf::Time GetMedianTime() const;

private:
    inline static constexpr uint16_t SamplesCountLimit { 99 };
    std::array<float, SamplesCountLimit> _samples;
    uint16_t _samplesCount { 0 };
    sf::Time _timePerTick;
};

}