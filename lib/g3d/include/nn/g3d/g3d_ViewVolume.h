#pragma once

#include <cstdint>

#include <nn/util/util_MathTypes.h>

namespace nn::g3d {
class Aabb;
class Sphere;

class ViewVolume {
public:
    void SetPerspective(float, float, float, float, const nn::util::neon::MatrixRowMajor4x3fType&);
    void SetFrustum(float, float, float, float, float, float,
                    const nn::util::neon::MatrixRowMajor4x3fType&);
    void SetOrtho(float, float, float, float, float, float,
                  const nn::util::neon::MatrixRowMajor4x3fType&);
    bool TestIntersection(const nn::g3d::Sphere&);
    int32_t TestIntersectionEx(const nn::g3d::Sphere&);
    bool TestIntersection(const nn::g3d::Aabb&);
    int32_t TestIntersectionEx(const nn::g3d::Aabb&);

private:
    void* filler[29];
};
}  // namespace nn::g3d
