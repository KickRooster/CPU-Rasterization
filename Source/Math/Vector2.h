#pragma once
#include "MathBase.h"

namespace core
{
    struct FVector2
    {
        float X;
        float Y;

        FVector2();
        FVector2(float InX, float InY);
        FVector2& operator= (const FVector2& Vector);
    };
}
