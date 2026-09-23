set(NN_ATK_HEADERS
  include/nn/atk.h
  include/nn/atk/atk_AuxBus.h
  include/nn/atk/atk_SoundArchivePlayer.h
  include/nn/atk/atk_SoundDataManager.h
  include/nn/atk/atk_SoundPlayer.h
  include/nn/atk/detail/atk_AdvancedWaveSoundRuntime.h
  include/nn/atk/detail/atk_BasicSound.h
  include/nn/atk/detail/atk_SequenceSoundRuntime.h
  include/nn/atk/detail/atk_SoundArchiveManager.h
  include/nn/atk/detail/atk_StreamSoundRuntime.h
  include/nn/atk/detail/atk_WaveSoundRuntime.h
)

set(NN_ATK_SOURCES
  src/atk/atk_Stub.cpp
)

add_library(nn_atk OBJECT ${NN_ATK_SOURCES})

target_sources(nn_atk PUBLIC
  FILE_SET HEADERS
  BASE_DIRS include/
  FILES ${NN_ATK_HEADERS}
)

set_target_properties(nn_atk PROPERTIES
    LINKER_LANGUAGE CXX
    VERIFY_INTERFACE_HEADER_SETS ON
)

target_include_directories(nn_atk PUBLIC include/)

target_nn_ver(nn_atk PUBLIC NN_WARE)
target_compile_options(nn_atk INTERFACE
    -fno-strict-aliasing
    -fno-exceptions
    -Wall
    -Wextra
    -Wno-invalid-offsetof
)
