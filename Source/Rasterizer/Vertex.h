#pragma once
#include <Core.h>
#include <../Math/Vector4.h>
#include "HDRColor.h"

namespace core
{
    struct FVertex
    {
        FVector4 Position;
        FHDRColor Color;
        //  TODO, normal tangent, e.g.

        FVertex();
        FVertex(float X, float Y, float Z, float W);
        FVertex(float X, float Y, float Z, float W, float R, float G, float B, float A);
        FVertex& operator= (const FVertex& Vertex);
    };
}
