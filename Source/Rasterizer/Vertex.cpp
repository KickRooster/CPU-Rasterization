#include "Vertex.h"

namespace core
{
    FVertex::FVertex():
    Position(0, 0, 0, 0)
    {
    }

    FVertex::FVertex(float X, float Y, float Z, float W):
    Position(X, Y, Z, W)
    {
    }

    FVertex& FVertex::operator=(const FVertex& Vertex)
    {
        Position = Vertex.Position;

        return *this;
    }
}
