#pragma once

#include <nn/gfx/gfx_Device.h>

namespace nn::g3d {
class MaterialObj {
public:
    struct InitializeArgument {
        void CalculateMemorySize();
    };

    bool Initialize(const InitializeArgument&, void*, uint64_t);
    void InitializeDependPointer();
    int64_t GetBlockBufferAlignment(gfx::TDevice<gfx::ApiVariationNvn8>*) const;
    int32_t CalculateBlockBufferSize(gfx::TDevice<gfx::ApiVariationNvn8>*) const;
    void SetupBlockBufferImpl(gfx::TDevice<gfx::ApiVariationNvn8>*,
                              gfx::TMemoryPool<gfx::ApiVariationNvn8>*, int64_t, uint64_t);
    void ResetDirtyFlags();
    bool SetupBlockBuffer(gfx::TDevice<gfx::ApiVariationNvn8>*,
                          gfx::TMemoryPool<gfx::ApiVariationNvn8>*, int64_t, uint64_t);
    void CleanupBlockBuffer(gfx::TDevice<gfx::ApiVariationNvn8>*);
    void CalculateMaterial(int32_t);

    template <bool>
    void ConvertDirtyParams(void*, uint32_t*);

private:
    void* filler[16];
};
}  // namespace nn::g3d
