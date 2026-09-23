/**
 * @file TexCoordArray.h
 * @brief Texture coordinate array implementation.
 */

#pragma once

#include <cstdint>

#include <nn/util/util_MathTypes.h>
#include <nn/ui2d/ui2d_Layout.h>

namespace nn::ui2d::detail {
class TexCoordArray {
public:
    void Initialize();
    void Free();
    void Reserve(int32_t);
    void SetSize(int32_t size);
    void GetCoord(nn::util::Float2*, int32_t) const;
    void SetCoord(int32_t, nn::util::Float2 const*);
    void Copy(void const*, int32_t);
    bool CompareCopiedInstanceTest(nn::ui2d::detail::TexCoordArray const&) const;

    uint16_t _0;
    uint16_t _2;
    uint32_t _4;                     // padding?
    nn::ui2d::Layout* mLayout;  // _8
};
} // namespace nn::ui2d::detail


