#pragma once
#include "Actor.h"
#include "../Math/Matrix4x4.h"

namespace core
{
    class UCamera : public UActor
    {
    protected:
        FMatrix4x4 PerspectiveProjectionMatrix;
        FMatrix4x4 OrthogonalProjectionMatrix;

    public:
        float Aspect;
        float FieldOfViewY;
        float ZNear;
        float ZFar;

        UCamera();
        void Tick(float DeltaTime) override;
        void BuildPerspectiveProjectionMatrix();
        const FMatrix4x4& GetPerspectiveProjectionMatrix() const ;
    };
}
