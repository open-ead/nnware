/**
 * @brief Sequenced Sound Runtime Info
 */

#pragma once

#include <cstdint>

namespace nn::atk::detail {
class SoundArchiveManager;

class SequenceSoundRuntime {
public:
    SequenceSoundRuntime();
    ~SequenceSoundRuntime();

    void Initialize(int32_t, void**, void const*);
    void Finalize();
    void SetupSequenceTrack(int32_t, void**, void const*);
    void SetupUserParam(void**, uint64_t);
    bool IsSoundArchiveAvailable() const;
    int32_t GetActiveCount() const;
    int32_t GetFreeCount() const;
    void SetSequenceSkipIntervalTick(int32_t tick);
    int32_t GetSequenceSkipIntervalTick();
    void Update();

    uint8_t _0[0xD0];
    nn::atk::detail::SoundArchiveManager* mArchiveManager;  // _D0
    uint64_t _d8;
};
} // namespace nn::atk::detail


