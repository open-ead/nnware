#pragma once

#include <cstdint>

namespace nn::g3d {

class BindFuncTable;

class ResLightAnim {
public:
    int32_t Bind(nn::g3d::BindFuncTable const&);
};

}  // namespace nn::g3d
