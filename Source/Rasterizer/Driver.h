#pragma once
#include "../DesignPattern/SingletonPattern/Singleton.h"
#include "../Objects/Actor.h"
#include "../Objects/Camera.h"
#include "../Math/Rect.h"
#include "Rasterizer.h"
#include "IrradianceBuffer.h"

namespace core
{
    enum EPrimitiveType
    {
        EPrimitiveType_Triangles,
        EPrimitiveType_Strip,
        EPrimitiveType_Fan,
    };

    enum EFace
    {
        EFace_Back,
        EFace_Front,
    };

    enum EWindingOrder
    {
        EWindingOrder_ClockWise,
        EWindingOrder_CounterClockWise,
    };

    class UDriver : public Singleton<UDriver>
    {
        FRect ViewportRect;
        FMatrix4x4 ViewportMat;

        void UpdateViewportMatrix();

        void TransformLocal2World(const UActor& Actor, const FMatrix4x4& Local2World) const;
        void TransformWorld2Camera(const UActor& Actor, const FMatrix4x4& World2Camera) const;
        void TransformCamera2CVV(const UActor& Actor, const FMatrix4x4& Projection) const;
        void ProjectCVV2NDC(const UActor& Actor) const;
        void TransformNDC2Viewport(const UActor& Actor, const FMatrix4x4& Viewport) const;
        void Rasterize(const UActor& Actor, FIrradianceBuffer& IrradianceBuffer) const;
        void Rasterize_Debug(const UActor& Actor, FIrradianceBuffer& IrradianceBuffer) const;

    public:
        void SetViewport(int32 X, int32 Y, int32 Width, int32 Height);
        void DrawActor(const UActor& Actor, const UCamera& Camera, FIrradianceBuffer& IrradianceBuffer);
        void Debug(const UActor& Actor, const UCamera& Camera, FIrradianceBuffer& IrradianceBuffer);
    };
}
