#include "Triangle.h"

namespace core
{
    FTriangle::FTriangle(const FVertex& InV0, const FVertex& InV1, const FVertex& InV2):
    V0(InV0),
    V1(InV1),
    V2(InV2)
    {
    }

    FTriangle& FTriangle::operator=(const FTriangle& Triangle)
    {
        V0 = Triangle.V0;
        V1 = Triangle.V1;
        V2 = Triangle.V2;

        return *this;
    }
}
