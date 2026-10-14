/**
 * @file ResMaterial.h
 * @brief Resource material for models.
 */

#pragma once

#include <cstring>

#include <nn/gfx/gfx_Types.h>

namespace nn {
namespace gfx {
class SamplerInfo;
}
namespace g3d {

struct TextureRef {
    void* pTexture;
    void* pUserData;
};

class ResShaderParam {
public:
    enum Type {
        kTypeBool1 = 0x00,
        kTypeBool2 = 1,
        kTypeBool3 = 2,
        kTypeBool4 = 3,
        kTypeS321 = 4,
        kTypeS322 = 5,
        kTypeS323 = 6,
        kTypeS324 = 7,
        kTypeU321 = 8,
        kTypeU322 = 9,
        kTypeU323 = 10,
        kTypeU324 = 11,
        kTypeF321 = 12,
        kTypeF322 = 13,
        kTypeF323 = 14,
        kTypeF324 = 15,
        kTypeF32_2x1 = 16,
        kTypeF32_2x2 = 17,
        kTypeF32_2x3 = 18,
        kTypeF32_2x4 = 19,
        kTypeF32_3x1 = 20,
        kTypeF32_3x2 = 21,
        kTypeF32_3x3 = 22,
        kTypeF32_3x4 = 23,
        kTypeF32_4x1 = 24,
        kTypeF32_4x2 = 25,
        kTypeF32_4x3 = 26,
        kTypeF32_4x4 = 27,
        kTypeSrt2d = 0x1c,
        kTypeSrt3d = 29,
        kTypeTexSrt = 30,
        kTypeTexSrtEx = 31,
    };

    typedef uint64_t (*ConvertFunc)(void* pDst, const void* pSrc, const ResShaderParam* pParam,
                                    const void* pUserData);

    template <bool T>
    void Convert(void* pDst, const void* pSrc) const {
        uint32_t type = m_Type;
        if (type <= kTypeF324) {
            memcpy(pDst, pSrc, ((type & 3) + 1) * sizeof(float));
            return;
        }
        if (type <= kTypeF32_4x4) {
            size_t srcRowSize = ((type & 3) + 1) * sizeof(float);
            int32_t rowCount = ((static_cast<int32_t>(type) - kTypeF32_2x1) >> 2) + 2;
            for (int32_t row = 0; row < rowCount; row++) {
                memcpy(pDst, pSrc, srcRowSize);
                pDst = static_cast<uint8_t*>(pDst) + 0x10;
                pSrc = static_cast<const uint8_t*>(pSrc) + srcRowSize;
            }
        }
    }

    static uint64_t GetSize(Type type);
    static uint64_t GetSrcSize(Type type);
    bool SetDependPointer(void* pDependPointer, const void* pSrc) const;
    bool GetDependPointer(void** ppDependPointer, const void* pSrc) const;

    static uint64_t ConvertSrt2dCallback(void* pDst, const void* pSrc, const ResShaderParam* pParam,
                                         const void* pUserData);
    static uint64_t ConvertSrt3dCallback(void* pDst, const void* pSrc, const ResShaderParam* pParam,
                                         const void* pUserData);
    static uint64_t ConvertSrt2dExCallback(void* pDst, const void* pSrc,
                                           const ResShaderParam* pParam, const void* pUserData);
    static uint64_t ConvertTexSrtCallback(void* pDst, const void* pSrc,
                                          const ResShaderParam* pParam, const void* pUserData);
    static uint64_t ConvertTexSrtExCallback(void* pDst, const void* pSrc,
                                            const ResShaderParam* pParam, const void* pUserData);

private:
    uint8_t _0[0x10];
    uint8_t m_Type;  // 0x10
    uint8_t _11;     // 0x11
    uint16_t _12;
    uint32_t _14;
    uint32_t _18;  // 0x18
    uint32_t _1c;
};

class ResMaterial {
public:
    struct ShaderParamEntry {
        ResShaderParam::ConvertFunc pConvertFunc;  // 0x00
        uint64_t _8;
        uint8_t type;  // 0x10
        uint8_t _11[3];
        int32_t _14;  // 0x14
        uint32_t _18;
        uint8_t _1c;  // 0x1c
        uint8_t _1d[3];
    };
    static_assert(sizeof(ShaderParamEntry) == 0x20);

    uint64_t BindTexture(nn::g3d::TextureRef (*)(char const*, void*), void*);
    bool ForceBindTexture(nn::g3d::TextureRef const&, char const*);
    void ReleaseTexture();
    void Setup(gfx::Device*);
    void Cleanup(gfx::Device*);
    void Reset();
    void Reset(uint32_t);

    uint8_t _0[0x30];
    void** m_pTextureRefs;               // 0x30
    const char* const* m_pTextureNames;  // 0x38
    gfx::Sampler* m_pSamplers;           // 0x40
    gfx::SamplerInfo* m_pSamplerInfos;   // 0x48
    uint64_t _50;
    ShaderParamEntry* m_pShaderParams;  // 0x58
    uint8_t _60[0x20];
    void* m_pShaderParamFlags;  // 0x80
    void* _88;                  // 0x88
    uint8_t _90[8];
    int64_t* m_pTextureStatus;  // 0x98
    uint8_t _a0[8];
    uint8_t m_TextureCount;  // 0xa8
    uint8_t _a9;
    uint16_t m_ShaderParamCount;  // 0xaa
    uint16_t _ac;
    uint16_t _ae;
    uint16_t _b0;
    uint16_t _b2;
};
}  // namespace g3d
}  // namespace nn
