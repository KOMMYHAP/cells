#pragma once

class SpinLock {
public:
    void Lock();
    void Unlock();
    bool TryLock();

private:
    std::atomic_bool _locked { false };
};

class SpinLocker {
public:
    SpinLocker(SpinLock& lock);
    SpinLocker(const SpinLocker&) = delete;
    SpinLocker& operator=(const SpinLocker&) = delete;
    SpinLocker(SpinLocker&&) = delete;
    SpinLocker& operator=(SpinLocker&&) = delete;

    ~SpinLocker();

private:
    gsl::not_null<SpinLock*> _lock;
};