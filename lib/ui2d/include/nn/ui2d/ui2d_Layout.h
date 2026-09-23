/**
 * @brief UI Layout implementation.
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace nn::ui2d {
class AnimTransform;
class Pane;

class Layout {
public:
    Layout();

    virtual ~Layout();

    virtual void DeleteAnimTransform(nn::ui2d::AnimTransform*);
    virtual void BindAnimation(nn::ui2d::AnimTransform*);
    virtual void UnbindAnimation(nn::ui2d::AnimTransform*);
    virtual void UnbindAnimation(nn::ui2d::Pane*);
    virtual void UnbindAllAnimation();

    virtual void Animate();
    virtual void UpdateAnimFrame(float frame);
    virtual void AnimateAndUpdateAnimFrame(float frame);

    static void SetAllocator(void* (*)(size_t, size_t, void*), void (*)(void*, void*), void*);
    static void AllocateMemory(size_t, size_t);
    static void AllocateMemory(size_t);
    static void FreeMemory(void* src);

    Pane* GetPane() const { return mPane; }

private:
    uint64_t _8;
    uint64_t _10;
    Pane* mPane;
    uint64_t _20;
    float _28;
    float _2c;
    uint64_t _30;

    uint64_t _40;
    uint64_t _48;
    uint64_t _50;
    uint64_t _58;
    uint64_t _60;

    static void* g_pAllocateFunction;
    static void* g_pFreeFunction;
};
}  // namespace nn::ui2d
