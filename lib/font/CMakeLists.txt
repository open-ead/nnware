set(NN_FONT_HEADERS
  include/nn/font/font_Util.h
)

set(NN_FONT_SOURCES
  src/font/font_Stub.cpp
)

add_library(nn_font OBJECT ${NN_FONT_SOURCES})

target_sources(nn_font PUBLIC
  FILE_SET HEADERS
  BASE_DIRS include/
  FILES ${NN_FONT_HEADERS}
)

set_target_properties(nn_font PROPERTIES
    LINKER_LANGUAGE CXX
    VERIFY_INTERFACE_HEADER_SETS ON
)

target_include_directories(nn_font PUBLIC include/)

target_nn_ver(nn_font PUBLIC NN_WARE)
target_compile_options(nn_font INTERFACE
    -fno-strict-aliasing
    -fno-exceptions
    -Wall
    -Wextra
    -Wno-invalid-offsetof
)
