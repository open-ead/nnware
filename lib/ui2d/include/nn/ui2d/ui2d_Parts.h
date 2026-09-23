/**
 * @brief Layout parts.
 */

#pragma once

#include <nn/ui2d/ui2d_Pane.h>

namespace nn::ui2d {
struct BuildArgSet;
struct ResParts;

class Parts : nn::ui2d::Pane {
public:
    NN_RUNTIME_TYPEINFO(nn::ui2d::Pane);

    Parts();
    Parts(nn::ui2d::ResParts const*, nn::ui2d::ResParts const*, nn::ui2d::BuildArgSet const&);
    Parts(nn::ui2d::Parts const&);

    virtual ~Parts();

    uint64_t _e0;
    uint64_t _e8;
    uint32_t _f0;
};
}  // namespace nn::ui2d
