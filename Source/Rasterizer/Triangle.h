#pragma once
#include "Core.h"
#include "Vertex.h"

namespace core
{
    struct FTriangle
    {
        FVertex V0;
        FVertex V1;
        FVertex V2;

        FTriangle(const FVertex& InV0, const FVertex& InV1, const FVertex& InV2);
        FTriangle& operator =(const FTriangle& Triangle);
    };
}
