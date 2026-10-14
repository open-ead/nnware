#pragma once

#include <cstddef>
#include <cstdint>

#include <nn/g3d/g3d_MaterialObj.h>
#include <nn/g3d/g3d_ViewVolume.h>

namespace nn::g3d {

class MaterialObj;
class ResModel;
class ShapeObj;
class SkeletonObj;

// TODO
class ModelObj {
public:
    MaterialObj* FindMaterial(const char* materialName);

    SkeletonObj* GetSkeleton() const { return m_Skeleton; }

    int32_t GetNumShapes() const { return m_NumShapes; }

    int32_t get_8c() const { return _8c; }

    uint8_t GetViewDependentModelFlags() const { return m_ViewDependentModelFlags; }

    MaterialObj* GetMaterial(int32_t index) const { return &m_Materials[index]; }

    const Sphere& GetBounds() const { return *m_Bounds; }

private:
    struct InitializeArgument;

    bool Initialize(const InitializeArgument& arg, void* buffer, size_t bufferSize);

    const ResModel* m_ResModel;
    void* _8;
    void* _10;
    uint8_t _18;
    uint8_t m_ViewDependentModelFlags;  // TODO: is this right?
    uint16_t _1a;
    void* _20;
    void* _28;
    uint16_t m_NumShapes;
    uint16_t m_NumMaterials;
    SkeletonObj* m_Skeleton;
    ShapeObj* m_Shapes;
    MaterialObj* m_Materials;
    Sphere* m_Bounds;
    void* m_UserData;
    void* _60;
    void* _68;
    void* _70;
    void* _78;
    void* _80;
    bool _88;
    int _8c;
};

}  // namespace nn::g3d
