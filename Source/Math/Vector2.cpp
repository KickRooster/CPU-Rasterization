#include "Vector2.h"

namespace core
{
    FVector2::FVector2():
    X(0),
    Y(0)
    {
    }

    FVector2::FVector2(float InX, float InY):
    X(InX),
    Y(InY)
    {
    }

    FVector2::FVector2(const FVector2& Vector):
    X(Vector.X),
    Y(Vector.Y)
    {

    }

    FVector2& FVector2::operator=(const FVector2& Vector)
    {
        X = Vector.X;
        Y = Vector.Y;

        return *this;
    }

    FVector2 FVector2::operator+(const FVector2& Vector) const
    {
        return FVector2(X + Vector.X, Y + Vector.Y);
    }

    FVector2 FVector2::operator-(const FVector2& Vector) const
    {
        return FVector2(X - Vector.X, Y - Vector.Y);
    }

    FVector2 FVector2::operator*(float Scale) const
    {
        return FVector2(X * Scale, Y * Scale);
    }

    FVector2 FVector2::operator/(float Scale) const
    {
        return FVector2(X / Scale, Y / Scale);
    }

    float FVector2::Cross(const FVector2& LHS, const FVector2& RHS)
    {
        return LHS.X * RHS.Y - RHS.X * LHS.Y;
    }
}
