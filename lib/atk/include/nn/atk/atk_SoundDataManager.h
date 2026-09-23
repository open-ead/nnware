/**
 * @brief Sound data management implementation.
 */

#pragma once

#include <cstdint>
namespace nn::atk {
class SoundDataManager {
public:
    SoundDataManager();
    virtual ~SoundDataManager();

    virtual void InvalidateData(void const*, void const*);
    virtual void SetFileAddressToTable(uint32_t, void const*);
    virtual uint64_t GetFileAddressFromTable(uint32_t) const;
    virtual uint32_t GetFileAddressImpl(uint32_t) const;

    uint8_t _0[0x240];
};
} // namespace nn::atk

