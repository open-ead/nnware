/**
 * @brief Resource material for models.
 */

#pragma once

#include <cstdint>

#include <nn/gfx/gfx_Types.h>

namespace nn::g3d {

typedef void* TextureRef;

class ResMaterial {
public:
    uint64_t BindTexture(nn::g3d::TextureRef (*)(char const*, void*), void*);
    void ForceBindTexture(nn::g3d::TextureRef const&, char const*);
    void ReleaseTexture();
    void Setup(gfx::Device*);
    void Cleanup(gfx::Device*);
    void Reset();
    void Reset(uint32_t);

    uint8_t _0[0xB4];
};

} // namespace nn::g3d

