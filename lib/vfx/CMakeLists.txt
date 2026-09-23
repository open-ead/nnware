find_nnsdk()
find_nnsdk_module(nn_gfx)

set(NN_VFX_HEADERS
  include/nn/vfx/vfx_Config.h
  include/nn/vfx/vfx_Heap.h
  include/nn/vfx/vfx_Resource.h
  include/nn/vfx/vfx_System.h
)

set(NN_VFX_SOURCES
  src/vfx/vfx_Stub.cpp
)

add_library(nn_vfx OBJECT ${NN_VFX_SOURCES})

target_sources(nn_vfx PUBLIC
  FILE_SET HEADERS
  BASE_DIRS include/
  FILES ${NN_VFX_HEADERS}
)

set_target_properties(nn_vfx PROPERTIES
    LINKER_LANGUAGE CXX
    VERIFY_INTERFACE_HEADER_SETS ON
)

target_include_directories(nn_vfx PUBLIC include/)

target_nn_ver(nn_vfx PUBLIC NN_WARE)
target_nn_ver(nn_gfx PUBLIC NN_WARE)
target_nn_ver(nnSdk INTERFACE NN_WARE)
target_compile_options(nn_vfx INTERFACE
    -fno-strict-aliasing
    -fno-exceptions
    -Wall
    -Wextra
    -Wno-invalid-offsetof
)
target_link_libraries(nn_vfx PUBLIC nn_g3d)
target_link_libraries(nn_vfx PUBLIC nn_gfx)
target_link_libraries(nn_vfx PUBLIC nnSdk)
