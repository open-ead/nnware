cmake_minimum_required(VERSION 3.15)

set(NN_G3D_HEADERS
)

set(NN_G3D_SOURCES
)

add_library(nn_g3d OBJECT ${NN_G3D_SOURCES})

target_sources(nn_g3d PUBLIC
  FILE_SET HEADERS
  BASE_DIRS include/
  FILES ${NN_G3D_HEADERS}
)

set_target_properties(nn_g3d PROPERTIES
    LINKER_LANGUAGE CXX
    VERIFY_INTERFACE_HEADER_SETS ON
)

target_include_directories(nn_g3d PUBLIC include/)

target_nn_ver(nn_g3d PUBLIC NN_WARE)
target_compile_options(nn_g3d INTERFACE
    -fno-strict-aliasing
    -fno-exceptions
    -Wall
    -Wextra
    -Wno-invalid-offsetof
)
