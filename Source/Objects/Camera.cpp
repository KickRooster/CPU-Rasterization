#include "Camera.h"
#include "../Math/Vector2.h"

namespace core
{
    UCamera::UCamera():
    PerspectiveProjectionMatrix(1.f),
    OrthogonalProjectionMatrix(1.f),
    LookAtDir(FVector3::Forward)
    {
    }

    void UCamera::RespondToInput(const FJoystick& Joystick, float DeltaTime)
    {
        float MoveSpeed = 0.01f * DeltaTime * static_cast<float>(Joystick.ZPos);

        if (Joystick.LeftShiftDown)
        {
            MoveSpeed *= 2.0f;
        }

        if (Joystick.WDown)
        {
            Position += World2LocalMatrix.GetZAxis() * MoveSpeed;
        }

        if (Joystick.SDown)
        {
            Position += World2LocalMatrix.GetZAxis() * -MoveSpeed;
        }

        if (Joystick.ADown)
        {
            Position += World2LocalMatrix.GetXAxis() * -MoveSpeed;
        }

        if (Joystick.DDown)
        {
            Position += World2LocalMatrix.GetXAxis() * MoveSpeed;
        }

        if (Joystick.QDown)
        {
            Position += World2LocalMatrix.GetYAxis() * -MoveSpeed;
        }

        if (Joystick.EDown)
        {
            Position += World2LocalMatrix.GetYAxis() * MoveSpeed;
        }

        static bool RightMouseClicked = false;
        static FVector2 RightMousePosPrev(0, 0);

        if (Joystick.RightButtonDown)
        {
            if (!RightMouseClicked)
            {
                RightMousePosPrev.X = static_cast<float>(Joystick.XPos);
                RightMousePosPrev.Y = static_cast<float>(Joystick.YPos);

                RightMouseClicked = true;
            }
            else
            {
                float XDelta = static_cast<float>(Joystick.XPos) - RightMousePosPrev.X;
                float YDelta = static_cast<float>(Joystick.YPos) - RightMousePosPrev.Y;

                World2LocalMatrix = FMatrix4x4::RotateAround(World2LocalMatrix, World2LocalMatrix.GetYAxis(),0.02f * DeltaTime * Degree2Radian(XDelta));
                World2LocalMatrix = FMatrix4x4::RotateAround(World2LocalMatrix, World2LocalMatrix.GetXAxis(),0.02f * DeltaTime * Degree2Radian(YDelta));

                LookAtDir = World2LocalMatrix.GetZAxis();

                RightMousePosPrev.X = static_cast<float>(Joystick.XPos);
                RightMousePosPrev.Y = static_cast<float>(Joystick.YPos);
            }
        }
        else
        {
            RightMouseClicked = false;
        }
    }

    void UCamera::Tick(float DeltaTime)
    {
        UActor::Tick(DeltaTime);
        World2LocalMatrix.LookAt(Position, Position + LookAtDir, FVector3::Up);
        Local2WorldMatrix = World2LocalMatrix.GetInverse();
        BuildPerspectiveProjectionMatrix();
    }

    //  D3D style, left-handed coordinate projection matrix.
    void UCamera::BuildPerspectiveProjectionMatrix()
    {
        PerspectiveProjectionMatrix.M[0][0] = 1.0f / (Aspect * tan(FieldOfViewY * 0.5f));
        PerspectiveProjectionMatrix.M[1][1] = 1.0f / tan(FieldOfViewY * 0.5f);
        PerspectiveProjectionMatrix.M[2][2] = ZFar / (ZFar - ZNear);
        PerspectiveProjectionMatrix.M[2][3] = 1.0f;
        PerspectiveProjectionMatrix.M[3][2] = -ZNear * ZFar / (ZFar - ZNear);
    }

    const FMatrix4x4& UCamera::GetPerspectiveProjectionMatrix() const
    {
        return PerspectiveProjectionMatrix;
    }
}
