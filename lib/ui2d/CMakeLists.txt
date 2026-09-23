cmake_minimum_required(VERSION 3.15)

set(NN_UI2D_HEADERS
)

set(NN_UI2D_SOURCES
)

add_library(nn_ui2d OBJECT ${NN_UI2D_SOURCES})

target_sources(nn_ui2d PUBLIC
  FILE_SET HEADERS
  BASE_DIRS include/
  FILES ${NN_UI2D_HEADERS}
)

set_target_properties(nn_ui2d PROPERTIES
    LINKER_LANGUAGE CXX
    VERIFY_INTERFACE_HEADER_SETS ON
)

target_include_directories(nn_ui2d PUBLIC include/)

target_nn_ver(nn_ui2d PUBLIC NN_WARE)
target_compile_options(nn_ui2d INTERFACE
    -fno-strict-aliasing
    -fno-exceptions
    -Wall
    -Wextra
    -Wno-invalid-offsetof
)
