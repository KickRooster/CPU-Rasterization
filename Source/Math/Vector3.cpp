#include "Vector3.h"

namespace core
{
    FVector3::FVector3():
    X(0),
    Y(0),
    Z(0)
    {
    }

    FVector3::FVector3(float X, float Y, float Z):
    X(X),
    Y(Y),
    Z(Z)
    {
    }

    FVector3& FVector3::operator=(const FVector3& Vector)
    {
        X = Vector.X;
        Y = Vector.Y;
        Z = Vector.Z;

        return *this;
    }
}
