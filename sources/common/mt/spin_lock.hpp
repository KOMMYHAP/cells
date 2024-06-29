#pragma once

#include <emmintrin.h> ///< _mm_pause

#include "spin_lock.h"

inline void SpinLock::Lock()
{
    while (true) {
        while (_locked.load(std::memory_order_relaxed)) {
            _mm_pause();
        }

        if (bool expected = false; _locked.compare_exchange_weak(expected, true, std::memory_order_release, std::memory_order_relaxed)) {
            break;
        }
    }
}

inline void SpinLock::Unlock()
{
    _locked.store(false, std::memory_order_acquire);
}

inline bool SpinLock::TryLock()
{
    if (!_locked.load(std::memory_order_relaxed)) {
        return false;
    }
    _locked.store(true, std::memory_order_release);
    return true;
}

inline SpinLocker::SpinLocker(SpinLock& lock)
    : _lock(&lock)
{
    _lock->Lock();
}

inline SpinLocker::~SpinLocker()
{
    _lock->Unlock();
}