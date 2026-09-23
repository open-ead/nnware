/**
 * @brief Stream sound runtime information.
 */

#pragma once

namespace nn::atk::detail {

class StreamSoundRuntime {
public:
    StreamSoundRuntime();
    ~StreamSoundRuntime();

    char _0[0xB0];
};

}  // namespace nn::atk::detail
