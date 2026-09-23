#pragma once

#include <cstdint>

namespace nn::g3d {
class ResFogAnim {
public:
    char mMagic[4];                // _0
    uint16_t mFlags;               // _4
    uint16_t mPad;                 // _6
    int32_t mNumFrames;            // _8
    uint8_t mNumCurves;            // _C
    uint8_t mIdxDistanceAttnFunc;  // _D
    uint16_t mNumUserData;         // _E
    uint32_t mSizeBaked;           // _10
    uint64_t mNameOffset;          // _14
    uint64_t mFuncNameOffset;      // _1C
};
}  // namespace nn::g3d
