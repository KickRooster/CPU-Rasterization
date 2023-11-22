#include "Camera.h"

namespace core
{
    UCamera::UCamera():
    PerspectiveProjectionMatrix(1.f),
    OrthogonalProjectionMatrix(1.f)
    {
    }

    void UCamera::Tick(float DeltaTime)
    {
        UActor::Tick(DeltaTime);
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
