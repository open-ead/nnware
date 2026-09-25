#include <nn/g3d/g3d_ResMaterial.h>

#include <arm_neon.h>
#include <new>

#include <nn/gfx/detail/gfx_Fwd.h>
#include <nn/gfx/gfx_Sampler.h>
#include <nn/gfx/gfx_SamplerInfo.h>
#include <nn/util/util_Arithmetic.h>

namespace nn::g3d {
namespace {

class BindResultX {
public:
    static BindResultX Bound() {
        BindResultX r;
        r.m_Flag = 1;
        return r;
    }
    static BindResultX NotBound() {
        BindResultX r;
        r.m_Flag = 0x10000;
        return r;
    }
    BindResultX& operator|=(const BindResultX& rhs) {
        m_Flag |= rhs.m_Flag;
        return *this;
    }
    uint32_t m_Flag{0};
};

const int64_t s_SizeTable[3] = {24, 48, 24};
const int64_t s_SrcSizeTable[4] = {20, 36, 24, 32};

uint64_t ConvertTexSrtMayaCallback(void* pDst, const void* pSrc, const ResShaderParam* pParam,
                                   const void* pUserData);
uint64_t ConvertTexSrtMaxCallback(void* pDst, const void* pSrc, const ResShaderParam* pParam,
                                  const void* pUserData);
uint64_t ConvertTexSrtXsiCallback(void* pDst, const void* pSrc, const ResShaderParam* pParam,
                                  const void* pUserData);
uint64_t ConvertTexSrtMayaExCallback(void* pDst, const void* pSrc, const ResShaderParam* pParam,
                                     const void* pUserData);
uint64_t ConvertTexSrtMaxExCallback(void* pDst, const void* pSrc, const ResShaderParam* pParam,
                                    const void* pUserData);
uint64_t ConvertTexSrtXsiExCallback(void* pDst, const void* pSrc, const ResShaderParam* pParam,
                                    const void* pUserData);

const ResShaderParam::ConvertFunc s_TexSrtConvertFuncs[8] = {
    ConvertTexSrtMayaCallback,
    ConvertTexSrtMaxCallback,
    ConvertTexSrtXsiCallback,
    ConvertTexSrtMayaExCallback,
    ConvertTexSrtMaxExCallback,
    ConvertTexSrtXsiExCallback,
    ResShaderParam::ConvertSrt2dCallback,
    ResShaderParam::ConvertSrt3dCallback,
};

const ResShaderParam::ConvertFunc s_DefaultConvertFuncs[4] = {
    ResShaderParam::ConvertSrt2dCallback,
    ResShaderParam::ConvertSrt3dCallback,
    ResShaderParam::ConvertTexSrtCallback,
    ResShaderParam::ConvertTexSrtExCallback,
};

}  // namespace

template void ResShaderParam::Convert<true>(void* pDst, const void* pSrc) const;
template void ResShaderParam::Convert<false>(void* pDst, const void* pSrc) const;

uint64_t ResShaderParam::GetSize(Type type) {
    if (type <= kTypeF324)
        return ((type & 3) + 1) * sizeof(float);
    if (type <= kTypeF32_4x4)
        return static_cast<int64_t>(((type - kTypeF32_2x1) >> 2) + 2) * 0x10;
    return s_SizeTable[type - kTypeSrt2d];
}

uint64_t ResShaderParam::GetSrcSize(Type type) {
    if (type <= kTypeF324)
        return ((type & 3) + 1) * sizeof(float);
    if (type <= kTypeF32_4x4)
        return static_cast<uint32_t>((type & 3) + 1) *
               static_cast<int64_t>(((type - kTypeF32_2x1) >> 2) + 2) * sizeof(float);
    return s_SrcSizeTable[type - kTypeSrt2d];
}

bool ResShaderParam::SetDependPointer(void* pDependPointer, const void* pSrc) const {
    uint8_t dependOffset = _11;
    uint64_t srcSize = GetSrcSize(static_cast<Type>(m_Type));
    if (dependOffset <= srcSize)
        return false;
    uint32_t value = _18;
    return static_cast<uint16_t>(value) == (value >> 16);
}

bool ResShaderParam::GetDependPointer(void** ppDependPointer, const void* pSrc) const {
    uint8_t dependOffset = _11;
    uint64_t srcSize = GetSrcSize(static_cast<Type>(m_Type));
    if (dependOffset <= srcSize) {
        *ppDependPointer = nullptr;
        return false;
    }
    *ppDependPointer = *reinterpret_cast<void* const*>(static_cast<const uint8_t*>(pSrc) + m_Type);
    return true;
}

uint64_t ResShaderParam::ConvertSrt2dCallback(void* pDst, const void* pSrc, const ResShaderParam*,
                                              const void*) {
    auto* dst = static_cast<float*>(pDst);
    auto* src = static_cast<const float*>(pSrc);
    util::AngleIndex angle = util::RadianToAngleIndex(src[2]);
    float sin = util::SinTable(angle);
    float cos = util::CosTable(angle);
    dst[0] = src[0] * cos;
    dst[1] = src[0] * sin;
    dst[2] = -(src[1] * sin);
    dst[3] = cos * src[1];
    dst[4] = src[3];
    dst[5] = src[4];
    return 24;
}

// NON_MATCHING: SIMD scheduling and register allocation (https://decomp.me/scratch/lkHXp)
uint64_t ResShaderParam::ConvertSrt3dCallback(void* pDst, const void* pSrc, const ResShaderParam*,
                                              const void*) {
    auto* dst = static_cast<float*>(pDst);
    auto* src = static_cast<const float*>(pSrc);
    float32x4_t translation = vdupq_n_f32(0.0f);
    float32x4_t scaleX = vld1q_dup_f32(src);
    float32x4_t rotation = translation;
    rotation = vld1q_lane_f32(src + 3, rotation, 0);
    rotation = vld1q_lane_f32(src + 4, rotation, 1);
    const float* cosCoefficients = util::detail::CosCoefficients;
    translation = vld1q_lane_f32(src + 6, translation, 0);
    const float32x4_t negHalf = vdupq_n_f32(-0.5f);
    translation = vld1q_lane_f32(src + 7, translation, 1);
    translation = vld1q_lane_f32(src + 8, translation, 2);
    rotation = vld1q_lane_f32(src + 5, rotation, 2);
    const float32x4_t half = vdupq_n_f32(0.5f);
    float32x4_t turns = vmulq_n_f32(rotation, util::detail::Float1Divided2Pi);
    uint32x4_t positive = vcgeq_f32(turns, vdupq_n_f32(0.0f));
    float32x4_t roundBias = vbslq_f32(positive, half, negHalf);
    turns = vaddq_f32(turns, roundBias);
    int32_t turn0 = static_cast<int32_t>(vgetq_lane_f32(turns, 0));
    int32x2_t lowTurns = vmov_n_s32(turn0);
    int32_t turn1 = static_cast<int32_t>(vgetq_lane_f32(turns, 1));
    const float* sinCoefficients = util::detail::SinCoefficients;
    int32_t turn3 = static_cast<int32_t>(vgetq_lane_f32(turns, 3));
    lowTurns = vset_lane_s32(turn1, lowTurns, 1);
    int32_t turn2 = static_cast<int32_t>(vgetq_lane_f32(turns, 2));
    int32x2_t highTurns = vmov_n_s32(turn2);
    highTurns = vset_lane_s32(turn3, highTurns, 1);
    float32x4_t sinC3 = vdupq_n_f32(sinCoefficients[3]);
    float32x2_t highRounded = vcvt_f32_s32(highTurns);
    float32x4_t sinC1 = vdupq_n_f32(sinCoefficients[1]);
    float32x4_t negOne = vdupq_n_f32(-1.0f);
    float32x2_t lowRounded = vcvt_f32_s32(lowTurns);
    float32x4_t cosC1 = vdupq_n_f32(cosCoefficients[1]);
    float32x4_t rounded = vcombine_f32(lowRounded, highRounded);
    float32x4_t negHalfPi = vdupq_n_f32(-util::detail::FloatPiDivided2);
    float32x4_t halfPi = vdupq_n_f32(util::detail::FloatPiDivided2);
    rotation = vfmsq_n_f32(rotation, rounded, util::detail::Float2Pi);
    float32x4_t negPi = vdupq_n_f32(-util::detail::FloatPi);
    uint32x4_t above = vcgtq_f32(rotation, halfPi);
    float32x4_t sinC2 = vdupq_n_f32(-sinCoefficients[2]);
    float32x4_t pi = vdupq_n_f32(util::detail::FloatPi);
    float32x4_t reflectedAbove = vsubq_f32(pi, rotation);
    rotation = vbslq_f32(above, reflectedAbove, rotation);
    uint16x4_t aboveNarrow = vmovn_u32(above);
    uint32x4_t below = vcltq_f32(rotation, negHalfPi);
    uint16x4_t belowNarrow = vmovn_u32(below);
    float32x4_t sinC4 = vdupq_n_f32(-sinCoefficients[4]);
    float32x4_t reflectedBelow = vsubq_f32(negPi, rotation);
    uint16x4_t reflected = vorr_u16(aboveNarrow, belowNarrow);
    rotation = vbslq_f32(below, reflectedBelow, rotation);
    float32x4_t square = vmulq_f32(rotation, rotation);
    float32x4_t sinPolynomial = vfmaq_n_f32(sinC1, square, -sinCoefficients[0]);
    int32x4_t signMask = vreinterpretq_s32_u32(vmovl_u16(reflected));
    float32x4_t cosPolynomial = vfmaq_f32(cosC1, square, vdupq_n_f32(-cosCoefficients[0]));
    signMask = vshlq_n_s32(signMask, 31);
    signMask = vshrq_n_s32(signMask, 31);
    float32x4_t one = vdupq_n_f32(1.0f);
    float32x4_t cosC3 = vdupq_n_f32(cosCoefficients[3]);
    sinPolynomial = vfmaq_f32(sinC2, sinPolynomial, square);
    float32x4_t cosC2 = vdupq_n_f32(-cosCoefficients[2]);
    sinPolynomial = vfmaq_f32(sinC3, sinPolynomial, square);
    cosPolynomial = vfmaq_f32(cosC2, cosPolynomial, square);
    sinPolynomial = vfmaq_f32(sinC4, sinPolynomial, square);
    cosPolynomial = vfmaq_f32(cosC3, cosPolynomial, square);
    sinPolynomial = vfmaq_f32(one, sinPolynomial, square);
    float32x4_t cosC4 = vdupq_n_f32(-cosCoefficients[4]);
    cosPolynomial = vfmaq_f32(cosC4, cosPolynomial, square);
    float32x4_t sin = vmulq_f32(rotation, sinPolynomial);
    float32x2_t sinY = vdup_lane_f32(vget_low_f32(sin), 1);
    float32x4_t sign = vbslq_f32(vreinterpretq_u32_s32(signMask), negOne, one);
    float32x2_t ySinLow = vset_lane_f32(vgetq_lane_f32(sin, 1), sinY, 1);
    float32x2_t sinZ = vget_high_f32(sin);
    cosPolynomial = vfmaq_f32(one, square, cosPolynomial);
    float32x4_t cos = vmulq_f32(cosPolynomial, sign);
    float32x2_t cosY = vdup_lane_f32(vget_low_f32(cos), 1);
    float32x2_t cosZ = vget_high_f32(cos);
    float32x2_t zSinCosLow = vzip1_f32(sinZ, cosZ);
    float32x2_t sinYZero = vset_lane_f32(0.0f, sinY, 1);
    float32x4_t zSinCos = vcombine_f32(zSinCosLow, vdup_n_f32(0.0f));
    float32x2_t cosYZero = vset_lane_f32(0.0f, cosY, 1);
    float32x2_t yCosLow = vset_lane_f32(vgetq_lane_f32(cos, 1), cosY, 1);
    float32x4_t zSinCosCosX = vmulq_f32(vdupq_n_f32(vgetq_lane_f32(cos, 0)), zSinCos);
    float32x2_t zCosSinLow = vzip1_f32(cosZ, sinZ);
    float32x4_t zCosSinY = vcombine_f32(zCosSinLow, sinYZero);
    float32x4_t ySin = vcombine_f32(ySinLow, (float32x2_t){1.0f, 0.0f});
    float32x4_t column1 = vmulq_f32(zSinCosCosX, (float32x4_t){-1.0f, 1.0f, 0.0f, 0.0f});
    float32x4_t zSinCosSinX = vmulq_f32(vdupq_n_f32(vgetq_lane_f32(sin, 0)), zSinCos);
    float32x4_t zCosSin = vcombine_f32(zCosSinLow, cosYZero);
    float32x4_t yCos = vcombine_f32(yCosLow, (float32x2_t){-1.0f, 0.0f});
    float32x4_t ySinZ = vmulq_f32(zCosSin, ySin);
    float32x4_t ySinZSinX = vmulq_f32(ySinZ, vdupq_n_f32(vgetq_lane_f32(sin, 0)));
    column1 = vaddq_f32(column1, ySinZSinX);
    column1 = vmulq_f32(vdupq_n_f32(src[1]), column1);
    float32x4_t ySinZCosX = vmulq_f32(ySinZ, vdupq_n_f32(vgetq_lane_f32(cos, 0)));
    float32x4_t column2 = vmulq_f32((float32x4_t){1.0f, -1.0f, 0.0f, 0.0f}, zSinCosSinX);
    column2 = vaddq_f32(column2, ySinZCosX);
    float32x4_t column0 = vmulq_f32(zCosSinY, yCos);
    column2 = vmulq_f32(vdupq_n_f32(src[2]), column2);
    column0 = vmulq_f32(scaleX, column0);
    float32x4_t even02 = vzip1q_f32(column0, column2);
    float32x4_t odd02 = vzip2q_f32(column0, column2);
    float32x4_t even13 = vzip1q_f32(column1, translation);
    float32x4_t odd13 = vzip2q_f32(column1, translation);
    vst1q_f32(dst, vzip1q_f32(even02, even13));
    vst1q_f32(dst + 4, vzip2q_f32(even02, even13));
    vst1q_f32(dst + 8, vzip1q_f32(odd02, odd13));
    return 48;
}

uint64_t ResShaderParam::ConvertSrt2dExCallback(void* pDst, const void* pSrc, const ResShaderParam*,
                                                const void*) {
    auto* dst = static_cast<float*>(pDst);
    auto* src = static_cast<const float*>(pSrc);
    util::AngleIndex angle = util::RadianToAngleIndex(src[3]);
    float sin = util::SinTable(angle);
    float cos = util::CosTable(angle);
    float sy = src[2];
    float sx = src[1];
    const float* parent = *reinterpret_cast<const float* const*>(src + 6);
    float sxCos = sx * cos;
    float sxSin = sx * sin;
    float syCos = sy * cos;
    float sySin = sy * sin;
    if (parent) {
        dst[0] = sxCos * parent[0] - sxSin * parent[4] + sx * parent[8];
        dst[4] = sySin * parent[0] + syCos * parent[4] + sy * parent[8];
        dst[1] = sxCos * parent[1] - sxSin * parent[5] + sx * parent[9];
        dst[5] = sySin * parent[1] + syCos * parent[5] + sy * parent[9];
        dst[2] = sxCos * parent[2] - sxSin * parent[6] + sx * parent[10];
        dst[6] = sySin * parent[2] + syCos * parent[6] + sy * parent[10];
        dst[3] = sxCos * parent[3] - sxSin * parent[7] + sx * parent[11];
        dst[7] = sySin * parent[3] + syCos * parent[7] + sy * parent[11];
        *reinterpret_cast<uint64_t*>(dst + 10) = *reinterpret_cast<const uint64_t*>(parent + 10);
        *reinterpret_cast<uint64_t*>(dst + 8) = *reinterpret_cast<const uint64_t*>(parent + 8);
    } else {
        dst[0] = sxCos;
        dst[4] = sySin;
        dst[1] = -sxSin;
        dst[5] = syCos;
        dst[2] = sx;
        dst[6] = sy;
        dst[7] = 0.0f;
        dst[8] = 0.0f;
        dst[9] = 0.0f;
        dst[11] = 0.0f;
        dst[3] = 0.0f;
        dst[10] = 1.0f;
    }
    return 48;
}

uint64_t ResShaderParam::ConvertTexSrtCallback(void* pDst, const void* pSrc,
                                               const ResShaderParam* pParam,
                                               const void* pUserData) {
    typedef uint64_t (*DispatchFunc)(void*, const void*);
    return reinterpret_cast<DispatchFunc>(
        s_TexSrtConvertFuncs[*static_cast<const uint32_t*>(pSrc)])(pDst, pSrc);
}

uint64_t ResShaderParam::ConvertTexSrtExCallback(void* pDst, const void* pSrc,
                                                 const ResShaderParam* pParam,
                                                 const void* pUserData) {
    typedef uint64_t (*DispatchFunc)(void*, const void*);
    return reinterpret_cast<DispatchFunc>(
        s_TexSrtConvertFuncs[*static_cast<const uint32_t*>(pSrc)])(pDst, pSrc);
}

uint64_t ResMaterial::BindTexture(TextureRef (*pBindFunc)(char const*, void*), void* pUserData) {
    BindResultX result;
    uint32_t count = m_TextureCount;
    for (uint32_t i = 0; i < count; i++) {
        if (m_pTextureRefs[i] == nullptr || m_pTextureStatus[i] == -1) {
            TextureRef ref = pBindFunc(m_pTextureNames[i] + 2, pUserData);
            m_pTextureRefs[i] = ref.pTexture;
            m_pTextureStatus[i] = reinterpret_cast<int64_t>(ref.pUserData);
            if (ref.pTexture != nullptr && ref.pUserData != reinterpret_cast<void*>(-1))
                result |= BindResultX::Bound();
            else
                result |= BindResultX::NotBound();
        }
    }
    return result.m_Flag;
}

bool ResMaterial::ForceBindTexture(const TextureRef& ref, char const* pName) {
    bool result = false;
    uint32_t count = m_TextureCount;
    for (uint32_t i = 0; i < count; i++) {
        if (strcmp(m_pTextureNames[i] + 2, pName) == 0) {
            m_pTextureRefs[i] = ref.pTexture;
            m_pTextureStatus[i] = reinterpret_cast<int64_t>(ref.pUserData);
            result = true;
        }
    }
    return result;
}

void ResMaterial::ReleaseTexture() {
    uint32_t count = m_TextureCount;
    for (uint32_t i = 0; i < count; i++) {
        m_pTextureRefs[i] = nullptr;
        m_pTextureStatus[i] = -1;
    }
}

void ResMaterial::Setup(gfx::Device* pDevice) {
    uint32_t texCount = m_TextureCount;
    for (uint32_t i = 0; i < texCount; i++) {
        gfx::SamplerInfo* pInfo = &m_pSamplerInfos[i];
        gfx::Sampler* pSampler = &m_pSamplers[i];
        new (pSampler) gfx::Sampler;
        pSampler->Initialize(pDevice, *pInfo);
        gfx::util::SetSamplerDebugLabel(pSampler, "g3d");
    }
    uint32_t paramCount = m_ShaderParamCount;
    for (uint32_t i = 0; i < paramCount; i++) {
        ShaderParamEntry* pEntry = &m_pShaderParams[i];
        if (pEntry->type >= ResShaderParam::kTypeSrt2d && pEntry->pConvertFunc == nullptr) {
            pEntry->pConvertFunc = s_DefaultConvertFuncs[pEntry->type - ResShaderParam::kTypeSrt2d];
            pEntry->_1c = 1;
        }
    }
}

void ResMaterial::Cleanup(gfx::Device* pDevice) {
    uint32_t count = m_TextureCount;
    for (uint32_t i = 0; i < count; i++) {
        if (m_pSamplers[i].ToData()->state) {
            gfx::Sampler* pSampler = &m_pSamplers[i];
            pSampler->Finalize(pDevice);
            pSampler->~TSampler();
        }
    }
}

void ResMaterial::Reset() {
    uint32_t paramCount = m_ShaderParamCount;
    for (unsigned long i = 0; (uint32_t)i < paramCount; i++) {
        ShaderParamEntry* pEntry = &m_pShaderParams[i];
        pEntry->_14 = -1;
        pEntry->pConvertFunc = nullptr;
        pEntry->_1c = 1;
    }
    _a9 = m_TextureCount;
    _b0 = 0;
    _88 = nullptr;
    _ac = 0;
    memset(m_pShaderParamFlags, 0, paramCount >> 5);
}

void ResMaterial::Reset(uint32_t option) {
    uint32_t paramCount = m_ShaderParamCount;
    for (uint32_t i = 0; i < paramCount; i++) {
        ShaderParamEntry* pEntry = &m_pShaderParams[i];
        pEntry->_14 = -1;
        pEntry->pConvertFunc = nullptr;
        pEntry->_1c = 1;
    }
    _a9 = m_TextureCount;
    _b0 = 0;
    if ((option & 1) == 0)
        _88 = nullptr;
    _ac = 0;
    memset(m_pShaderParamFlags, 0, paramCount >> 5);
}

namespace {

uint64_t ConvertTexSrtMayaCallback(void* pDst, const void* pSrc, const ResShaderParam*,
                                   const void*) {
    auto* dst = static_cast<float*>(pDst);
    auto* src = static_cast<const float*>(pSrc);
    util::AngleIndex angle = util::RadianToAngleIndex(src[3]);
    float sin = util::SinTable(angle);
    float cos = util::CosTable(angle);
    float a = sin * 0.5f - 0.5f;
    float b = cos * -0.5f;
    dst[0] = src[1] * cos;
    dst[1] = -(src[2] * sin);
    dst[2] = src[1] * sin;
    dst[3] = cos * src[2];
    dst[4] = src[1] * (b - a - src[4]);
    dst[5] = src[2] * (b + a + src[5]) + 1.0f;
    return 24;
}

uint64_t ConvertTexSrtMaxCallback(void* pDst, const void* pSrc, const ResShaderParam*,
                                  const void*) {
    auto* dst = static_cast<float*>(pDst);
    auto* src = static_cast<const float*>(pSrc);
    util::AngleIndex angle = util::RadianToAngleIndex(src[3]);
    float sin = util::SinTable(angle);
    float cos = util::CosTable(angle);
    float sx = src[1];
    float sy = src[2];
    float sxCos = sx * cos;
    float sxSin = sx * sin;
    float syCos = sy * cos;
    float sySin = sy * sin;
    dst[0] = sxCos;
    dst[1] = -sySin;
    dst[2] = sxSin;
    dst[3] = syCos;
    dst[4] = -sxCos * (src[4] + 0.5f) + sxSin * (src[5] - 0.5f) + 0.5f;
    dst[5] = sySin * (src[4] + 0.5f) + syCos * (src[5] - 0.5f) + 0.5f;
    return 24;
}

uint64_t ConvertTexSrtXsiCallback(void* pDst, const void* pSrc, const ResShaderParam*,
                                  const void*) {
    auto* dst = static_cast<float*>(pDst);
    auto* src = static_cast<const float*>(pSrc);
    util::AngleIndex angle = util::RadianToAngleIndex(src[3]);
    float sin = util::SinTable(angle);
    float cos = util::CosTable(angle);
    float sx = src[1];
    float sy = src[2];
    float sxCos = sx * cos;
    float sxSin = sx * sin;
    float syCos = sy * cos;
    float sySin = sy * sin;
    dst[0] = sxCos;
    dst[1] = sySin;
    dst[2] = -sxSin;
    dst[3] = syCos;
    dst[4] = sxSin - sxCos * src[4] - sxSin * src[5];
    dst[5] = -syCos - sySin * src[4] + syCos * src[5] + 1.0f;
    return 24;
}

uint64_t ConvertTexSrtMayaExCallback(void* pDst, const void* pSrc, const ResShaderParam*,
                                     const void*) {
    auto* dst = static_cast<float*>(pDst);
    auto* src = static_cast<const float*>(pSrc);
    util::AngleIndex angle = util::RadianToAngleIndex(src[3]);
    float sin = util::SinTable(angle);
    float cos = util::CosTable(angle);
    float sx = src[1];
    float sy = src[2];
    float sxCos = sx * cos;
    float syCos = sy * cos;
    float sxSin = sx * sin;
    float sySin = sy * sin;
    float tx = src[4];
    float x = (sxCos + sxSin - sx) * -0.5f - sx * tx;
    float yh = sy + (syCos - sySin);
    float ty = src[5];
    float y = yh * -0.5f + sy * ty + 1.0f;
    const float* parent = *reinterpret_cast<const float* const*>(src + 6);
    if (parent) {
        dst[0] = sxCos * parent[0] + sxSin * parent[4] + x * parent[8];
        dst[4] = -sySin * parent[0] + syCos * parent[4] + y * parent[8];
        dst[1] = sxCos * parent[1] + sxSin * parent[5] + x * parent[9];
        dst[5] = -sySin * parent[1] + syCos * parent[5] + y * parent[9];
        dst[2] = sxCos * parent[2] + sxSin * parent[6] + x * parent[10];
        dst[6] = -sySin * parent[2] + syCos * parent[6] + y * parent[10];
        dst[3] = sxCos * parent[3] + sxSin * parent[7] + x * parent[11];
        dst[7] = -sySin * parent[3] + syCos * parent[7] + y * parent[11];
        *reinterpret_cast<uint64_t*>(dst + 10) = *reinterpret_cast<const uint64_t*>(parent + 10);
        *reinterpret_cast<uint64_t*>(dst + 8) = *reinterpret_cast<const uint64_t*>(parent + 8);
    } else {
        dst[0] = sxCos;
        dst[11] = 0.0f;
        dst[4] = -sySin;
        dst[1] = sxSin;
        dst[5] = syCos;
        dst[2] = x;
        dst[6] = y;
        dst[7] = 0.0f;
        dst[8] = 0.0f;
        dst[9] = 0.0f;
        dst[3] = 0.0f;
        dst[10] = 1.0f;
    }
    return 48;
}

uint64_t ConvertTexSrtMaxExCallback(void* pDst, const void* pSrc, const ResShaderParam*,
                                    const void*) {
    auto* dst = static_cast<float*>(pDst);
    auto* src = static_cast<const float*>(pSrc);
    util::AngleIndex angle = util::RadianToAngleIndex(src[3]);
    float sin = util::SinTable(angle);
    float cos = util::CosTable(angle);
    float sx = src[1];
    float sy = src[2];
    float ty = src[5];
    float sxCos = sx * cos;
    float sxSin = sx * sin;
    float syCos = sy * cos;
    float tx = src[4];
    float sySin = sy * sin;
    float txH = tx + 0.5f;
    float txSxCos = txH * sxCos;
    float tyH = ty - 0.5f;
    float x = tyH * sxSin - txSxCos + 0.5f;
    float tySyCos = tyH * syCos;
    float txSySin = txH * sySin;
    float y = txSySin + tySyCos + 0.5f;
    const float* parent = *reinterpret_cast<const float* const*>(src + 6);
    if (parent) {
        dst[0] = sxCos * parent[0] + sxSin * parent[4] + x * parent[8];
        dst[4] = -sySin * parent[0] + syCos * parent[4] + y * parent[8];
        dst[1] = sxCos * parent[1] + sxSin * parent[5] + x * parent[9];
        dst[5] = -sySin * parent[1] + syCos * parent[5] + y * parent[9];
        dst[2] = sxCos * parent[2] + sxSin * parent[6] + x * parent[10];
        dst[6] = -sySin * parent[2] + syCos * parent[6] + y * parent[10];
        dst[3] = sxCos * parent[3] + sxSin * parent[7] + x * parent[11];
        dst[7] = -sySin * parent[3] + syCos * parent[7] + y * parent[11];
        *reinterpret_cast<uint64_t*>(dst + 10) = *reinterpret_cast<const uint64_t*>(parent + 10);
        *reinterpret_cast<uint64_t*>(dst + 8) = *reinterpret_cast<const uint64_t*>(parent + 8);
    } else {
        dst[0] = sxCos;
        dst[11] = 0.0f;
        dst[5] = syCos;
        dst[1] = sxSin;
        dst[2] = x;
        dst[6] = y;
        dst[7] = 0.0f;
        dst[8] = 0.0f;
        dst[9] = 0.0f;
        dst[3] = 0.0f;
        dst[10] = 1.0f;
        dst[4] = -sySin;
    }
    return 48;
}

uint64_t ConvertTexSrtXsiExCallback(void* pDst, const void* pSrc, const ResShaderParam*,
                                    const void*) {
    using namespace nn::util::detail;
    auto* dst = static_cast<float*>(pDst);
    auto* src = static_cast<const float*>(pSrc);

    unsigned int angle = static_cast<unsigned int>(
        static_cast<long long>(src[3] * (static_cast<float>(AngleIndexHalfRound) / FloatPi)));
    const SinCosSample& s = SinCosSampleTable[(angle >> 24) & 0xff];
    float rest = static_cast<float>(angle & 0xffffff) * (1.0f / 0x1000000);
    float sin = s.sinValue + s.sinDelta * rest;
    float cos = s.cosValue + s.cosDelta * rest;

    float sx = src[1];
    float sxCos = sx * cos;
    float sxSin = sx * sin;
    float sy = src[2];
    float syCos = sy * cos;
    float txSxCos = src[4] * sxCos;
    float x = sxSin - txSxCos - src[5] * sxSin;
    float sySin = sy * sin;
    float txSySin = src[4] * sySin;
    float y = -syCos - txSySin + src[5] * syCos + 1.0f;
    const float* parent = *reinterpret_cast<const float* const*>(src + 6);
    if (parent) {
        dst[0] = sxCos * parent[0] - sxSin * parent[4] + x * parent[8];
        dst[4] = sySin * parent[0] + syCos * parent[4] + y * parent[8];
        dst[1] = sxCos * parent[1] - sxSin * parent[5] + x * parent[9];
        dst[5] = sySin * parent[1] + syCos * parent[5] + y * parent[9];
        dst[2] = sxCos * parent[2] - sxSin * parent[6] + x * parent[10];
        dst[6] = sySin * parent[2] + syCos * parent[6] + y * parent[10];
        dst[3] = sxCos * parent[3] - sxSin * parent[7] + x * parent[11];
        dst[7] = sySin * parent[3] + syCos * parent[7] + y * parent[11];
        *reinterpret_cast<uint64_t*>(dst + 10) = *reinterpret_cast<const uint64_t*>(parent + 10);
        *reinterpret_cast<uint64_t*>(dst + 8) = *reinterpret_cast<const uint64_t*>(parent + 8);
    } else {
        dst[0] = sxCos;
        dst[1] = -sxSin;
        dst[5] = syCos;
        dst[2] = x;
        dst[6] = y;
        dst[7] = 0.0f;
        *reinterpret_cast<uint64_t*>(dst + 8) = 0;
        dst[3] = 0.0f;
        dst[10] = 1.0f;
        dst[11] = 0.0f;
        dst[4] = sySin;
    }
    return 48;
}

}  // namespace
}  // namespace nn::g3d
