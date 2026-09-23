/**
 * @file Pane.h
 * @brief Base UI panel.
 */

#pragma once

#include <nn/font/font_Util.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/ui2d/ui2d_Types.h>
#include <nn/util.h>
#include <nn/util/util_IntrusiveList.h>
#include <nn/util/util_MathTypes.h>

namespace nn::ui2d::detail {

class PaneBase {
    NN_NO_COPY(PaneBase);

public:
    PaneBase();
    virtual ~PaneBase();

    util::IntrusiveListNode m_Link;
};

}  // namespace nn::ui2d::detail

namespace nn::ui2d {
class AnimTransform;
class Layout;
class DrawInfo;
class ResPane;
struct BuildArgSet;
class Material;
struct ResExtUserDataList;

class Pane : public detail::PaneBase {
public:
    NN_RUNTIME_TYPEINFO_BASE();

    struct CalculateContext;

    typedef util::IntrusiveList<Pane, util::IntrusiveListMemberNodeTraits<
                                          detail::PaneBase, &detail::PaneBase::m_Link, Pane>>
        PaneList;

    Pane();
    Pane(const ResPane*, const BuildArgSet&);
    Pane(const Pane&);

    ~Pane() override;
    virtual void Finalize(gfx::Device*);
    virtual int32_t GetVertexColor(int32_t);
    virtual void SetVertexColor(int32_t, util::Unorm8x4 const&);
    virtual uint8_t GetColorElement(int32_t);
    virtual void SetColorElement(uint32_t, uint8_t);
    virtual uint8_t GetVertexColorElement(int32_t);
    virtual void SetVertexColorElement(uint32_t, uint8_t);
    virtual uint32_t GetMaterialCount() const;
    virtual Material* GetMaterial(int32_t) const;
    virtual Pane* FindPaneByName(char const*, bool);
    virtual const Pane* FindPaneByName(char const*, bool) const;
    virtual Material* FindMaterialByName(char const*, bool);
    virtual const Material* FindMaterialByName(char const*, bool) const;
    virtual void BindAnimation(AnimTransform*, bool, bool);
    virtual void UnbindAnimation(AnimTransform*, bool);
    virtual void UnbindAnimationSelf(AnimTransform*);
    virtual void Calculate(DrawInfo&, CalculateContext&, bool);
    virtual void Draw(DrawInfo&, gfx::CommandBuffer&);
    virtual void DrawSelf(DrawInfo&, gfx::CommandBuffer&);

    void SetName(const char*);
    void SetUserData(const char*);
    Material* GetMaterial() const;
    void AppendChild(Pane*);
    void PrependChild(Pane*);
    void InsertChild(Pane*, Pane*);
    void RemoveChild(Pane*);

    void Show() { SetVisible(true); }
    void Hide() { SetVisible(false); }

    bool IsVisible() const { return detail::TestBit(mFlags, PaneFlag_Visible); }
    bool IsInfluencedAlpha() const { return detail::TestBit(mFlags, PaneFlag_InfluencedAlpha); }
    bool IsLocationAdjust() const { return detail::TestBit(mFlags, PaneFlag_LocationAdjust); }
    bool IsUserAllocated() const { return detail::TestBit(mFlags, PaneFlag_UserAllocated); }
    bool IsGlobalMatrixDirty() const {
        return detail::TestBit(mFlags, PaneFlag_IsGlobalMatrixDirty);
    }
    bool IsUserMatrix() const { return detail::TestBit(mFlags, PaneFlag_UserMatrix); }
    bool IsUserGlobalMatrix() const { return detail::TestBit(mFlags, PaneFlag_UserGlobalMatrix); }
    bool IsConstantBufferReady() const {
        return detail::TestBit(mFlags, PaneFlag_IsConstantBufferReady);
    }
    bool IsMaxPanelFlag() const { return detail::TestBit(mFlags, PaneFlag_MaxPaneFlag); }

    const util::Float3& GetPosition() const { return mPosition; }
    void SetPosition(const util::Float3& position) {
        mPosition = position;
        SetGlobalMatrixDirty(true);
    }

    const util::Float3& GetRotation() const { return mRotation; }
    void SetRotation(const util::Float3& rotation) {
        mRotation = rotation;
        SetGlobalMatrixDirty(true);
    }

    const util::Float2& GetScale() const { return mScale; }
    void SetScale(const util::Float2& scale) {
        mScale = scale;
        SetGlobalMatrixDirty(true);
    }

    const Size& GetSize() const { return mSize; }
    void SetSize(const Size& size) {
        mSize.Set(size.width, size.height);
        SetGlobalMatrixDirty(true);
    }

    void SetAlpha(uint8_t alpha) { mAlpha = alpha; }

    uint8_t GetGlobalAlpha() const { return mGlobalAlpha; }

    const util::MatrixT4x3fType& GetMtx() const { return mMtx; }

protected:
    virtual void LoadMtx(DrawInfo&);
    virtual Pane* FindPaneByNameRecursive(const char*);
    virtual const Pane* FindPaneByNameRecursive(const char*) const;
    virtual Material* FindMaterialByNameRecursive(const char*);
    virtual const Material* FindMaterialByNameRecursive(const char*) const;

    const util::Float2& GetVertexPos() const;

    void SetVisible(bool state) { detail::SetBit(&mFlags, PaneFlag_Visible, state); }
    void SetInfluencedAlpha(bool state) {
        detail::SetBit(&mFlags, PaneFlag_InfluencedAlpha, state);
    }
    void SetLocationAdjust(bool state) { detail::SetBit(&mFlags, PaneFlag_LocationAdjust, state); }
    void SetUserAllocated(bool state) { detail::SetBit(&mFlags, PaneFlag_UserAllocated, state); }
    void SetGlobalMatrixDirty(bool state) {
        detail::SetBit(&mFlags, PaneFlag_IsGlobalMatrixDirty, state);
    }
    void SetUserMatrix(bool state) { detail::SetBit(&mFlags, PaneFlag_UserMatrix, state); }
    void SetUserGlobalMatrix(bool state) {
        detail::SetBit(&mFlags, PaneFlag_UserGlobalMatrix, state);
    }
    void setConstantBufferReady(bool state) {
        detail::SetBit(&mFlags, PaneFlag_IsConstantBufferReady, state);
    }
    void setMaxPanelFlag(bool state) { detail::SetBit(&mFlags, PaneFlag_MaxPaneFlag, state); }

private:
    void Initialize();
    const Pane& operator=(const Pane&);
    void CalculateScaleFromPartsRoot(util::Float2*, Pane*) const;
    void AllocateAndCopyAnimatedExtUserData(const ResExtUserDataList*);
    void CalculateGlobalMatrixSelf(CalculateContext&);

    Pane* mParent;
    PaneList mChildList;
    util::Float3 mPosition;
    util::Float3 mRotation;
    util::Float2 mScale;
    Size mSize;
    uint8_t mFlags;
    uint8_t mAlpha;
    uint8_t mGlobalAlpha;
    uint8_t mBasePosition;
    uint8_t mFlagEx;
    uint32_t mSystemDataFlags;
    Layout* mLayout;
    util::MatrixT4x3fType mMtx;
    const util::MatrixT4x3fType* mUserMtx;
    const ResExtUserDataList* mExtUserDataList;
    void* mAnimExtUserData;
    char mPanelName[25];
    char mUserData[9];
    uint16_t _da;
    uint32_t _dc;
};
}  // namespace nn::ui2d
