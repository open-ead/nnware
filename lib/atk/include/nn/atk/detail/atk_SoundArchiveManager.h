/**
 * @brief Sound archive manager implementation.
 */

#pragma once

#include <cstdint>

namespace nn::atk {

class SoundHandle;
class SoundArchive;
class SoundDataManager;

namespace detail {
class AddonSoundArchiveContainer;

class SoundArchiveManager {
public:
    SoundArchiveManager();

    virtual ~SoundArchiveManager();

    void Initialize(nn::atk::SoundArchive const*, nn::atk::SoundDataManager const*);
    void ChangeTargetArchive(char const*);
    void Finalize();
    bool IsAvailable() const;
    nn::atk::detail::AddonSoundArchiveContainer* GetAddonSoundArchive(char const*) const;

    uint64_t _8;
    uint64_t* _10;
    nn::atk::detail::AddonSoundArchiveContainer* _18;
    uint64_t* _20;
    nn::atk::SoundArchive* mSoundArchive;  // _28
    uint64_t _30;
    uint64_t _38;
    uint64_t _40;
};
}  // namespace detail
}  // namespace nn::atk
