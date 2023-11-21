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

        FTriangle(const FVertex& V0, const FVertex& V1, const FVertex& V2);
        FTriangle& operator =(const FTriangle& Triangle);
    };
}
