#pragma once
#include "sample_counter.h"
#include "simple_widget.h"

class FpsWidget final : public SimpleWidget {
public:
    void Update(sf::Time elapsedTime) override;

private:
    common::SampleCounter<int32_t, 30> _frameTimeCounter;
};