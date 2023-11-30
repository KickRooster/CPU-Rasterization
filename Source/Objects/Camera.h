#pragma once
#include "Actor.h"
#include "../Math/Vector3.h"
#include "../Math/Matrix4x4.h"
#include "../Others/Joystick.h"

namespace core
{
    class UCamera : public UActor
    {
    protected:
        FVector3 LookAtDir;
        FMatrix4x4 PerspectiveProjectionMatrix;
        FMatrix4x4 OrthogonalProjectionMatrix;

    public:
        float Aspect;
        float FieldOfViewY;
        float ZNear;
        float ZFar;

        UCamera();
        //  Called ahead of Tick.
        void RespondToInput(const FJoystick& Joystick, float DeltaTime);
        void Tick(float DeltaTime) override;
        void BuildPerspectiveProjectionMatrix();
        const FMatrix4x4& GetPerspectiveProjectionMatrix() const ;
    };
}
