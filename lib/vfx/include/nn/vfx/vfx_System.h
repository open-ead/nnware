/**
 * @brief VFX system implementation.
 */

#pragma once

#include <nn/vfx/vfx_Config.h>

// this class is massive

namespace nn::vfx {

class Heap;

class System {
public:
    System(nn::vfx::Config const&);

    virtual ~System();
    virtual void Initialize(nn::vfx::Heap*, nn::vfx::Heap*, nn::vfx::Config const&);

    char _0[0x1700];
};
} // namespace nn::vfx

