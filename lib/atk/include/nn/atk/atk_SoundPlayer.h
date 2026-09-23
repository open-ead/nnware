/**
 * @brief Sound player.
 */

#pragma once

#include <cstdint>

namespace nn::atk {
enum PauseMode {

};

class SoundPlayer {
public:
    SoundPlayer();
    ~SoundPlayer();

    void StopAllSound(int32_t);
    void Update();
    void DoFreePlayerHeap();
    void detail_SortPriorityList(bool);
    void PauseAllSound(int32_t, bool);
    void PauseAllSound(bool, int32_t, nn::atk::PauseMode);
    void SetVolume(float vol);
    void SetLowPassFilterFrequency(float filterFreq);
    void SetBiquadFilter(int32_t filterType, float baseFreq);
    void SetDefaultOutputLine(uint32_t line);

    void detail_SetPlayableSoundLimit(int32_t limit);
    bool CanPlaySound(int32_t);

    uint64_t _0;
    uint64_t _8;
    uint64_t _10;
    uint64_t _18;
    uint64_t _20;
    uint64_t _28;
    uint64_t _30;
    uint64_t _38;
    int32_t _40;
    int32_t mPlayableSoundCount;  // _44
    int32_t _48;
    float mVolume;                // _4C
    float mLowPassFreq;           // _50
    int32_t mFilterType;          // _54
    float mBaseFreq;              // _58
    uint32_t mDefaultOutputLine;  // _5C
    float mOutputVolume;          // _60
    uint64_t _64;
    uint64_t _6C;
};
}  // namespace nn::atk
