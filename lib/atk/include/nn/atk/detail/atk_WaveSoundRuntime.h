/**
 * @file WaveSoundRuntime.h
 * @brief Wave sound runtime info.
 */

#pragma once

#include <cstdint>

namespace nn::atk::detail {

class WaveSoundRuntime {
public:
    WaveSoundRuntime();
    ~WaveSoundRuntime();

    void Initialize(int32_t, void**, void const*);
    void Finalize();
    int32_t GetActiveCount() const;
    int32_t GetFreeWaveSoundCount() const;
    void SetupUserParam(void**, uint64_t);
    void Update();

    uint8_t _0[0x80];
};

} // namespace nn::atk::detail


