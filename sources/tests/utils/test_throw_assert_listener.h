#pragma once

class ThrowTestListener final : public testing::EmptyTestEventListener {
    void OnTestPartResult(const testing::TestPartResult& result) override;
};