/**
 * @brief Runtime wave sound api.
 */

#pragma once

#include <cstdint>

namespace nn::atk::detail {
class AdvancedWaveSoundRuntime {
public:
    AdvancedWaveSoundRuntime();
    ~AdvancedWaveSoundRuntime();

    void Initialize(int32_t, void**, void const*);
    void Finalize();
    int32_t GetActiveCount() const;
    void SetupUserParam(void**, uint64_t);
    void Update();

    uint8_t _0[0x30];
};
}  // namespace nn::atk::detail
