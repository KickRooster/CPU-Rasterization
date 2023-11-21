#include "Triangle.h"

namespace core
{
    FTriangle::FTriangle(const FVertex& V0, const FVertex& V1, const FVertex& V2)
    {
        this->V0 = V0;
        this->V1 = V1;
        this->V2 = V2;
    }

    FTriangle& FTriangle::operator=(const FTriangle& Triangle)
    {
        V0 = Triangle.V0;
        V1 = Triangle.V1;
        V2 = Triangle.V2;

        return *this;
    }
}
