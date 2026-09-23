cmake_minimum_required(VERSION 3.15)

set(NN_VFX_HEADERS
)

set(NN_VFX_SOURCES
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
target_compile_options(nn_vfx INTERFACE
    -fno-strict-aliasing
    -fno-exceptions
    -Wall
    -Wextra
    -Wno-invalid-offsetof
)
