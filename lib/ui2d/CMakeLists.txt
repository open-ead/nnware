find_nnsdk()
find_nnsdk_module(nn_gfx)

set(NN_UI2D_HEADERS
  include/nn/ui2d/detail/ui2d_TexCoordArray.h
  include/nn/ui2d/ui2d_Layout.h
  include/nn/ui2d/ui2d_Material.h
  include/nn/ui2d/ui2d_Pane.h
  include/nn/ui2d/ui2d_Parts.h
  include/nn/ui2d/ui2d_Types.h
  include/nn/ui2d/ui2d_Util.h
)

set(NN_UI2D_SOURCES
  src/ui2d/ui2d_Stub.cpp
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
target_nn_ver(nn_gfx PUBLIC NN_WARE)
target_nn_ver(nnSdk INTERFACE NN_WARE)
target_compile_options(nn_ui2d INTERFACE
    -fno-strict-aliasing
    -fno-exceptions
    -Wall
    -Wextra
    -Wno-invalid-offsetof
)

target_link_libraries(nn_ui2d PUBLIC nn_font)
target_link_libraries(nn_ui2d PUBLIC nn_gfx)
target_link_libraries(nn_ui2d PUBLIC nnSdk)
