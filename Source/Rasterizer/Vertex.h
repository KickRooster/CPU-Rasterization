#pragma once
#include <Core.h>
#include <../Math/Vector4.h>

namespace core
{
    struct FVertex
    {
        FVector4 Position;
        //  TODO, normal tangent, e.g.

        FVertex();
        FVertex(float X, float Y, float Z, float W);
        FVertex& operator= (const FVertex& Vertex);
    };
}
