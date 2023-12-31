#pragma once
#include <Core.h>
#include <../Math/Vector4.h>
#include <../Math/Vector2.h>
#include "HDRColor.h"

namespace core
{
    struct FVertex
    {
        FVector4 Position;
        FHDRColor Color;
        FVector2 UV;
        //  TODO, normal tangent, e.g.
        float CameraSpaceZ;     //  Used for perspective corrected interpolation.

        FVertex();
        FVertex(float X, float Y, float Z, float W);
        FVertex(float X, float Y, float Z, float W, float R, float G, float B, float A);
        FVertex(const FVector4& InPosition, const FHDRColor& InColor, const FVector2& InUV);
        FVertex& operator= (const FVertex& Vertex);
    };
}
