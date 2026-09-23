/**
 * @brief A basic sound.
 */

#pragma once

#include <cstdint>

namespace nn::atk {
class SoundActor;
class SoundPlayer;

enum MixMode {

};

namespace detail {
class PlayerHeap;
class ExternalSoundPlayer;

class BasicSound {
public:
    BasicSound();
    virtual ~BasicSound();

    virtual void Initialize();
    virtual void Finalize();
    virtual bool IsPrepared() const = 0;
    virtual bool IsAttachedTempSpecialHandle() = 0;
    virtual void DetachTempSpecialHandle() = 0;
    virtual void OnUpdatePlayerPriority();
    virtual void UpdateMoveValue();
    virtual void OnUpdateParam();

    void SetPriority(int32_t, int32_t);
    void GetPriority(int32_t*, int32_t*) const;
    void ClearIsFinalizedForCannotAllocatedResourceFlag();
    void SetId(uint32_t newID);
    bool IsAttachedGeneralHandle();
    void DetachGeneralHandle();
    bool IsAttachedTempGeneralHandle();
    void DetachTempGeneralHandle();
    void StartPrepared();
    void Stop(int32_t);
    void SetPlayerPriority(int32_t);
    void ForceStop();
    void Pause(bool, int32_t);
    void Mute(bool, int32_t);
    void SetAutoStopCounter(int32_t);
    void FadeIn(int32_t);
    bool IsPause() const;
    bool IsMute() const;
    void Update();
    void UpdateParam();
    void CalculateVolume() const;
    float CalculatePitch() const;
    float CalculateLpfFrequency() const;
    uint32_t CalculateOutLineFlag() const;
    void CalculateBiquadFilter(int32_t*, float*) const;
    void AttachPlayerHeap(nn::atk::detail::PlayerHeap*);
    void DetachPlayerHeap(nn::atk::detail::PlayerHeap*);
    void AttachSoundPlayer(nn::atk::SoundPlayer*);
    void DetachSoundPlayer(nn::atk::SoundPlayer*);
    void AttachSoundActor(nn::atk::SoundActor*);
    void DetachSoundActor(nn::atk::SoundActor*);
    void AttachExternalSoundPlayer(nn::atk::detail::ExternalSoundPlayer*);
    void DetachExternalSoundPlayer(nn::atk::detail::ExternalSoundPlayer*);
    uint32_t GetRemainingFadeFrames() const;
    uint32_t GetRemainingPauseFadeFrames() const;
    uint32_t GetRemainingMuteFadeFrames() const;
    void SetInitialVolume(float vol);
    float GetInitialVolume() const;
    void SetVolume(float, int32_t);
    int32_t GetVolume() const;
    void SetPitch(float);
    float GetPitch() const;
    void SetLpfFreq(float);
    float GetLpfFreq() const;
    void SetBiquadFilter(int32_t, float);
    void GetBiquadFilter(int32_t*, float*) const;
    void SetOutputLine(uint32_t);
    uint32_t GetOutputLine() const;
    void ResetOutputLine();
    void SetMixMode(nn::atk::MixMode);
    nn::atk::MixMode GetMixMode();
    void SetPan(float);
    float GetPan() const;
    void SetSurroundPan(float);
    float GetSurroundPan() const;
    void SetMainSend(float);
    float GetMainSend() const;

    uint64_t* _8;                        // nn::atk::detail::PlayerHeap*
    uint64_t* _10;                       // nn::atk::SoundHandle*
    uint64_t* _18;                       // nn::atk::SoundHandle*
    nn::atk::SoundPlayer* mSoundPlayer;  // _20
    uint64_t* _28;                       // nn::atk::SoundActor*
    uint64_t* _30;                       // nn::atk::detail::ExternalSoundPlayer*
    uint64_t* _38;                       // nn::atk::SoundArchive*
    uint8_t _40[0xF0 - 0x40];
    int32_t mPriority;  // _F0
    uint32_t _f4;
    uint32_t _f8;
    int32_t mAutoStopCounter;  // _FC
    uint64_t _100;
    uint32_t mID;  // _108
    uint32_t _10C;
    uint32_t _110;
    uint32_t _114;
    float mInitialVolume;  // _118
    float mPitch;          // _11C
    float mLpfFreq;        // _120
    float _124;
    uint32_t mOutputLine;  // _128
    float _12C;
    float mVolume;  // _130
    uint32_t _134;
    uint32_t _138;
    nn::atk::MixMode mMixMode;  // _13C
    float mPan;                 // _140
    float mSurroundPan;         // _144
    float mMainSend;            // _148
    uint8_t _14C[0x158 - 0x14C];
    float mOutputVol;  // _158
    uint8_t _15C[0x190 - 0x15C];
    float mOutputPan;          // _190
    float mOutputSurroundPan;  // _194
    float mOutputMainSend;     // _198
    float mOutputFxSend;       // _19C

    static uint64_t g_LastInstanceId;
};
}  // namespace detail
}  // namespace nn::atk
