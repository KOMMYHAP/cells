﻿#pragma once
#include "base_widget.h"
#include "sample_counter.h"

class FpsWidget final : public BaseWidget {
public:
    void UpdateWidget(sf::Time elapsedTime) override;

private:
    common::SampleCounter<int32_t, 30> _frameTimeCounter;
};