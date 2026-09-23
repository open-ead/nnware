/**
 * @brief UI Material implementation.
 */

#pragma once

#include <cstdint>

namespace nn::ui2d {
class AnimTransform;
class BuildResultInformation;
struct UserShaderInformation;

class Material {
public:
    Material();

    void Initialize();
    void ReserveMem(int32_t, int32_t, int32_t, int32_t, bool, int32_t, bool, int32_t, bool, bool);
    void SetupUserShaderConstantBufferInformation(nn::ui2d::UserShaderInformation const&);

    virtual ~Material();
    virtual void BindAnimation(nn::ui2d::AnimTransform*);
    virtual void UnbindAnimation(nn::ui2d::AnimTransform*);
};
}  // namespace nn::ui2d
