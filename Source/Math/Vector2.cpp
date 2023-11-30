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

    FVector2& FVector2::operator=(const FVector2& Vector)
    {
        X = Vector.X;
        Y = Vector.Y;

        return *this;
    }
}
