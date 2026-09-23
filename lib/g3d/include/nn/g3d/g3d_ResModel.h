/**
 * @file ResModel.h
 * @brief Resource model.
 */

#pragma once

#include <nn/gfx/gfx_Types.h>
#include <cstdint>

namespace nn::g3d {
class ResMaterial;

typedef void* TextureRef;

class ResModel {
public:
    uint64_t BindTexture(nn::g3d::TextureRef (*)(char const*, void*), void*);
    void ForceBindTexture(nn::g3d::TextureRef const&, char const*);
    void ReleaseTexture();
    void Setup(gfx::Device*);
    void Cleanup(gfx::Device*);
    void Reset();
    void Reset(uint32_t);
    nn::g3d::ResMaterial* FindMaterial(char const* materialName) const;

    uint8_t _0[0x70];
};

} // namespace nn::g3d

