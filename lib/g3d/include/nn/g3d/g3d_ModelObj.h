#pragma once

#include <cstddef>
#include <cstdint>

namespace nn::g3d {

class MaterialObj;
class ResModel;
class ShapeObj;
class SkeletonObj;

// TODO
class ModelObj {
public:
    SkeletonObj* GetSkeleton() const { return m_Skeleton; }

    int32_t GetNumShapes() const { return m_NumShapes; }

    int32_t get_8c() const { return _8c; }

private:
    struct InitializeArgument;

    bool Initialize(const InitializeArgument& arg, void* buffer, size_t bufferSize);

    const ResModel* m_ResModel;
    void* _8;
    void* _10;
    uint8_t _18;
    uint8_t _19;
    uint16_t _1a;
    void* _20;
    void* _28;
    uint16_t m_NumShapes;
    uint16_t m_NumMaterials;
    SkeletonObj* m_Skeleton;
    ShapeObj* m_Shapes;
    MaterialObj* m_Materials;
    void* _50;
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
