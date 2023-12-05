#pragma once
#include "MathBase.h"
#include "Vector2.h"
#include "Vector3.h"

namespace core
{
    bool Equal(float LHS, float RHS);
    float Degree2Radian(float Degree);
    float Radian2Degree(float Radian);
    float GetLength(const FVector2& Vector);
    float Cross(const FVector2& LHS, const FVector2& RHS);
    float GetArea(const FVector2& V0, const FVector2& V1, const FVector2& V2);
}
