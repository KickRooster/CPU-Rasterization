#include "Vertex.h"

namespace core
{
    FVertex::FVertex():
    Position(0, 0, 0, 0)
    {
    }

    FVertex::FVertex(float X, float Y, float Z, float W):
    Position(X, Y, Z, W),
    Color(0, 0, 0, 1)
    {
    }

    FVertex::FVertex(float X, float Y, float Z, float W, float R, float G, float B, float A):
    Position(X, Y, Z, W),
    Color(R, G, B, A)
    {
    }

    FVertex& FVertex::operator=(const FVertex& Vertex)
    {
        Position = Vertex.Position;
        Color = Vertex.Color;

        return *this;
    }
}
