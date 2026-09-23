#pragma once

#include <cstdint>

#include <nn/g3d/g3d_ModelObj.h>
#include <nn/g3d/g3d_ResModel.h>

namespace nn::g3d {
class AnimFrameCache;

class AnimBindTable {
public:
    void Initialize(uint32_t*, int32_t);
    void ClearAll(int32_t);
    void BindAll(uint16_t);

private:
    uint32_t* _0;
    int16_t _8;
    int16_t _a;
    int16_t _c;
    int16_t _e;
};
static_assert(sizeof(AnimBindTable) == 0x10);

class AnimFrameCtrl {
public:
    using PlayFunc = float (*)(float, float, float, void*);

    void Initialize(float, float, PlayFunc);
    static float PlayOneTime(float, float, float, void*);
    static float PlayLoop(float, float, float, void*);

    void update() { mFrame = mPlayFunc(mFrame + mFrameRate, _4, mFrameMax, _18); }

    float getFrame() const { return mFrame; }

    void setFrame(float frame) { mFrame = mPlayFunc(frame, _4, mFrameMax, _18); }

    float getFrameMax() const { return mFrameMax; }

    float getFrameRate() const { return mFrameRate; }

    void setFrameRate(float rate) { mFrameRate = rate; }

    bool isEnd() const { return mFrameMax <= mFrame; }

    bool isOneTime() const { return mPlayFunc == PlayOneTime; }

private:
    float mFrame;
    float _4;
    float mFrameMax;
    float mFrameRate;
    PlayFunc mPlayFunc;
    void* _18;
};
static_assert(sizeof(AnimFrameCtrl) == 0x20);

class AnimContext {
public:
    void Initialize(AnimFrameCache*, int32_t);

private:
    AnimFrameCache* mAnimFrameCache;
    int32_t _8;
    int32_t _c;
    int32_t _10;
};
static_assert(sizeof(AnimContext) == 0x18);

class AnimObj {
public:
    enum class BindFlag;

    virtual ~AnimObj();

    virtual void ClearResult();
    virtual void Calculate();
    virtual int32_t Bind(const ResModel*);
    virtual int32_t Bind(const ModelObj*);
    virtual void BindFast(const ResModel*);
    virtual void ApplyTo(ModelObj*) const;

    void ResetFrameCtrl(int32_t, bool);

    AnimFrameCtrl* getFrameCtrlPtr() const { return mFrameCtrlPtr; }

private:
    AnimFrameCtrl* mFrameCtrlPtr;
    AnimFrameCtrl mFrameCtrl;
    AnimContext mAnimContext;
    void* _48;
    void* _50;
};
static_assert(sizeof(AnimObj) == 0x58);

class ModelAnimObj : public AnimObj {
public:
    void SetBindFlagImpl(int32_t, AnimObj::BindFlag);
    const AnimObj::BindFlag& GetBindFlagImpl(int32_t) const;

private:
    AnimBindTable mBindTable;
};
static_assert(sizeof(ModelAnimObj) == 0x68);

}  // namespace nn::g3d
