cmake_minimum_required(VERSION 3.15)

find_nnsdk()
find_nnsdk_module(nn_gfx)

set(NN_G3D_HEADERS
  include/nn/g3d/g3d_AnimObj.h
  include/nn/g3d/g3d_BindFuncTable.h
  include/nn/g3d/g3d_BoneVisibilityAnimObj.h
  include/nn/g3d/g3d_ModelObj.h
  include/nn/g3d/g3d_ResFile.h
  include/nn/g3d/g3d_ResFogAnim.h
  include/nn/g3d/g3d_ResLightAnim.h
  include/nn/g3d/g3d_ResMaterialAnim.h
  include/nn/g3d/g3d_ResMaterial.h
  include/nn/g3d/g3d_ResModel.h
  include/nn/g3d/g3d_ResSceneAnim.h
  include/nn/g3d/g3d_ResShapeAnim.h
  include/nn/g3d/g3d_ResSkeletalAnim.h
  include/nn/g3d/g3d_ResSkeleton.h
  include/nn/g3d/g3d_SkeletonObj.h
  include/nn/g3d/g3d_ViewVolume.h
  include/nn/g3d/g3d_World.h
)

set(NN_G3D_SOURCES
  src/g3d/g3d_Stub.cpp
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
target_nn_ver(nn_gfx PUBLIC NN_WARE)
target_nn_ver(nnSdk INTERFACE NN_WARE)
target_compile_options(nn_g3d INTERFACE
    -fno-strict-aliasing
    -fno-exceptions
    -Wall
    -Wextra
    -Wno-invalid-offsetof
)

target_link_libraries(nn_g3d PUBLIC nn_gfx)
target_link_libraries(nn_g3d PUBLIC nnSdk)
