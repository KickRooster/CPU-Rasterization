#pragma once
#include "MathBase.h"

namespace core
{
    struct FVector3
    {
        float X;
        float Y;
        float Z;

        FVector3();
        FVector3(float X, float Y, float Z);
        FVector3& operator= (const FVector3& Vector);
    };
}
