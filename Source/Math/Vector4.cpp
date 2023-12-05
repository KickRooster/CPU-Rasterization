#include "Vector4.h"

namespace core
{
    FVector4::FVector4():
    X(0),
    Y(0),
    Z(0),
    W(0)
    {
    }

    FVector4::FVector4(float InX, float InY, float InZ, float InW):
    X(InX),
    Y(InY),
    Z(InZ),
    W(InW)
    {
    }

    FVector4::FVector4(const FVector4& Vector):
    X(Vector.X),
    Y(Vector.Y),
    Z(Vector.Z),
    W(Vector.W)
    {
    }

    FVector4& FVector4::operator=(const FVector4& Vector)
    {
        X = Vector.X ;
        Y = Vector.Y ;
        Z = Vector.Z ;
        W = Vector.W ;

        return *this;
    }

    FVector4 FVector4::operator*(const FMatrix4x4& Matrix)
    {
        FVector4 Vec;

        Vec.X = X * Matrix.M[0][0] + Y * Matrix.M[1][0] + Z * Matrix.M[2][0] + W * Matrix.M[3][0];
        Vec.Y = X * Matrix.M[0][1] + Y * Matrix.M[1][1] + Z * Matrix.M[2][1] + W * Matrix.M[3][1];
        Vec.Z = X * Matrix.M[0][2] + Y * Matrix.M[1][2] + Z * Matrix.M[2][2] + W * Matrix.M[3][2];
        Vec.W = X * Matrix.M[0][3] + Y * Matrix.M[1][3] + Z * Matrix.M[2][3] + W * Matrix.M[3][3];

        return Vec;
    }

    const FVector4& FVector4::operator*=(const FMatrix4x4& Matrix)
    {
        FVector4 Vec = (*this) * Matrix;

        X = Vec.X;
        Y = Vec.Y;
        Z = Vec.Z;
        W = Vec.W;

        return *this;
    }
}
