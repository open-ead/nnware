/**
 * @brief Resource file for models.
 */

#pragma once

#include <cstdint>

#include <nn/gfx/gfx_Types.h>
#include <nn/util.h>
#include <nn/util/util_BinaryFormat.h>

namespace nn {

namespace gfx {
template <typename T>
class TDevice;
}

namespace g3d {
class ResModel;
class ResMaterialAnim;
class ResShapeAnim;
class ResSceneAnim;
typedef void* TextureRef;

class ResFile : public nn::util::BinaryFileHeader {
public:
    static bool IsValid(void const* modelSrc);
    void Relocate();
    void Unrelocate();
    static nn::g3d::ResFile* ResCast(void*);
    int32_t BindTexture(nn::g3d::TextureRef (*ref)(char const*, void*), void*);
    void ReleaseTexture();
    void Setup(gfx::Device*);
    void Setup(gfx::Device*, gfx::MemoryPool*, int64_t, uint64_t);
    void Cleanup(gfx::Device*);
    void Reset();

    uint64_t mFileNameLength;                  // _20
    nn::g3d::ResModel* mModels;           // _28
    uint64_t mModelDictOffset;                 // _30
    uint64_t mSkeleAnimOffset;                 // _38
    uint64_t mSkeleAnimDictOffset;             // _40
    nn::g3d::ResMaterialAnim* mMatAnims;  // _48
    uint64_t mMatAnimsDictOffset;              // _50
    uint64_t mBoneVisiOffset;                  // _58
    uint64_t mBoneVisiDictOffset;              // _60
    nn::g3d::ResShapeAnim* mShapeAnims;   // _68
    uint64_t mShapeAnimDictOffset;             // _70
    nn::g3d::ResSceneAnim* mSceneAnims;   // _78
    uint64_t mSceneAnimDictOffset;             // _80
    uint64_t mMemoryPool;                      // _88
    uint64_t mBufferSection;                   // _90
    uint64_t mEmbeddedFilesOffset;             // _98
    uint64_t mEmbeddedFilesDictOffset;         // _A0
    uint64_t mPadding;                         // _A8
    uint64_t mStrTableOffset;                  // _B0
    uint32_t mStrTableSize;                    // _B8
    uint16_t mModelCount;                      // _BC
    uint16_t mSkeleAnimCount;                  // _BE
    uint16_t mMatAnimCount;                    // _C0
    uint16_t mBoneAnimCount;                   // _C2
    uint16_t mShapeAnimCount;                  // _C4
    uint16_t mSceneAnimCount;                  // _C6
    uint16_t mExternalFileCount;               // _C8
    uint8_t mPad[0x6];                         // _CA
};
}  // namespace g3d
}  // namespace nn
