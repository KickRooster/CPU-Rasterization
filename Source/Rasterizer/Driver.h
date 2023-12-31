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

        void Transform(FPrimitiveList& PrimitiveList, const FMatrix4x4& Matrix) const;
        void RecordCameraSpaceZ(FPrimitiveList& PrimitiveList) const;
        void Project2NDC(FPrimitiveList& PrimitiveList) const;
        void Rasterize(FPrimitiveList& PrimitiveList, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const;

    public:
        void SetViewport(int32 X, int32 Y, int32 Width, int32 Height);
        void DrawActor(UActor& Actor, const UCamera& Camera, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer);
    };
}
