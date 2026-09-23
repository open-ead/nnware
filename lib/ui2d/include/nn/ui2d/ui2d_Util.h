#pragma once

#include <nn/gfx/gfx_Device.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/util/util_MathTypes.h>

namespace nn::ui2d {

class AnimTransform;
class DrawInfo;
class Group;
class Layout;
class Pane;
class Material;
struct ResHermiteKey;
class ResourceTextureInfo;
class ShaderInfo;
struct ResExtUserDataList;
class ResExtUserData;

bool LoadTexture(ResourceTextureInfo*, gfx::Device*, const void*);
void LoadArchiveShader(ShaderInfo*, gfx::Device*, void*, const void*, gfx::MemoryPool*, int64_t,
                       uint64_t);
void FreeArchiveShader(gfx::Device*, ShaderInfo*);
void ConvertBlendsToArchiveShaderName(char*, int32_t, int32_t);
bool ConvertArchiveShaderNameToBlends(int32_t*, int32_t*, const char*);
int16_t SearchShaderVariationIndexFromTable(const void*, int32_t, int32_t);
void BindAnimation(AnimTransform*, Group*, bool);
void UnbindAnimation(AnimTransform*, Group*);
bool IsContain(const Pane*, const util::Float2&);
Pane* FindHitPane(Pane*, const util::Float2&);
const Pane* FindHitPane(const Pane*, const util::Float2&);
Pane* FindHitPane(Layout*, const util::Float2&);
const Pane* FindHitPane(const Layout*, const util::Float2&);
Pane* GetNextPane(Pane*);
Pane* ClonePaneTree(const Pane*, gfx::Device*);
Pane* ClonePaneTreeWithPartsLayout(const Pane*, Layout*, gfx::Device*, Layout*);
bool ComparePaneTreeTest(const Pane*, const Pane*);
void DrawNullAndBoundingPane(gfx::CommandBuffer&, DrawInfo&, Material&, const Pane*,
                             const util::Unorm8x4&, const util::Unorm8x4&);
float GetHermiteCurveValue(float, const ResHermiteKey*, int32_t);
const ResExtUserData* GetExtUserData(const ResExtUserDataList*, const char*);
uint64_t GetAlignedBufferSize(gfx::Device*, gfx::GpuAccess, uint64_t);
void SetDefaultShaderId(Material*, int32_t);

}  // namespace nn::ui2d
