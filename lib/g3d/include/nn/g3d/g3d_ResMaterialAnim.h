/**
 * @brief Resource file for material animations.
 */

#pragma once

#include <cstdint>

namespace nn::g3d {

struct TextureRef;

class ResMaterialAnim {
public:
    void ReleaseTexture();
    int32_t BindTexture(nn::g3d::TextureRef (*)(char const*, void*), void*);
    void Reset();
};

}  // namespace nn::g3d
