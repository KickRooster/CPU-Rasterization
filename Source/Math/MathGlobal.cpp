#include "MathGlobal.h"

namespace core
{
    bool Equal(float LHS, float RHS)
    {
        return abs(LHS - RHS) < FLOAT_ERROR;
    }

    float Degree2Radian(float Degree)
    {
        return Degree * DEGREE_TO_RADIAN;
    }

    float Radian2Degree(float Radian)
    {
        return Radian * RADIAN_TO_DEGREE;
    }

    float GetLength(const FVector2& Vector)
    {
        return sqrt(Vector.X * Vector.X + Vector.Y * Vector.Y);
    }

    float Cross(const FVector2& LHS, const FVector2& RHS)
    {
        return LHS.X * RHS.Y - RHS.X * LHS.Y;
    }

    float GetArea(const FVector2& V0, const FVector2& V1, const FVector2& V2)
    {
        return abs( Cross(FVector2(V1 - V0), FVector2(V2 - V0))) * 0.5f;
    }
}
