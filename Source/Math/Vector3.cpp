#include "Vector3.h"

namespace core
{
    FVector3::FVector3():
    X(0),
    Y(0),
    Z(0)
    {
    }

    FVector3::FVector3(float InX, float InY, float InZ):
    X(InX),
    Y(InY),
    Z(InZ)
    {
    }

    FVector3& FVector3::operator=(const FVector3& Vector)
    {
        X = Vector.X;
        Y = Vector.Y;
        Z = Vector.Z;

        return *this;
    }

    FVector3 FVector3::operator+(const FVector3& Vector3)
    {
        return FVector3(X + Vector3.X, Y + Vector3.Y, Z + Vector3.Z);
    }

    FVector3 FVector3::operator-(const FVector3& Vector) const
    {
        return FVector3(X - Vector.X, Y - Vector.Y, Z - Vector.Z);
    }

    FVector3 FVector3::operator*(float Scale) const
    {
        return FVector3(X * Scale, Y * Scale, Z * Scale);
    }

    FVector3 FVector3::operator-() const
    {
        return FVector3(-X, -Y, -Z);
    }

    const FVector3& FVector3::operator+=(const FVector3& Vector)
    {
        X += Vector.X;
        Y += Vector.Y;
        Z += Vector.Z;

        return *this;
    }

    float FVector3::GetLength() const
    {
        return sqrt(X * X + Y * Y + Z * Z);
    }

    void FVector3::Normalize()
    {
        float Length = GetLength();

        X /= Length;
        Y /= Length;
        Z /= Length;
    }

    FVector3 FVector3::Normalize(const FVector3& Vector)
    {
        float Length = Vector.GetLength();

        FVector3 Result = FVector3(Vector.X / Length, Vector.Y / Length, Vector.Z / Length);

        return Result;
    }

    float FVector3::Dot(const FVector3& LHS, const FVector3& RHS)
    {
        return LHS.X * RHS.X + LHS.Y * RHS.Y + LHS.Z * RHS.Z;
    }

    FVector3 FVector3::Cross(const FVector3& LHS, const FVector3& RHS)
    {
        //  left-hand rule
        return FVector3(
        RHS.Y * LHS.Z - RHS.Z * LHS.Y,
        RHS.Z * LHS.X - RHS.X * LHS.Z,
        RHS.X * LHS.Y - RHS.Y * LHS.X
            );

        //  right-hand rule
        return FVector3(
            LHS.Y * RHS.Z - LHS.Z * RHS.Y,
            LHS.Z * RHS.X - LHS.X * RHS.Z,
            LHS.X * RHS.Y - LHS.Y * RHS.X
            );
    }

    FVector3 FVector3::Forward = FVector3(0, 0, 1.0f);
    FVector3 FVector3::Up = FVector3(0, 1.0f, 0);
    FVector3 FVector3::Right = FVector3(1.0f, 0, 0);
}
