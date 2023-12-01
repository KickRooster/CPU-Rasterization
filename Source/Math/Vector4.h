#pragma once
#include "MathBase.h"
#include "Matrix4x4.h"

namespace core
{
    struct FVector4
    {
        float X;
        float Y;
        float Z;
        float W;

        FVector4();
        FVector4(float InX, float InY, float InZ, float InW);
        FVector4& operator= (const FVector4& Vector);
        FVector4 operator * (const FMatrix4x4& Matrix);
        const FVector4& operator *= (const FMatrix4x4& Matrix);
    };
}
