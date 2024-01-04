#include "LDRColor.h"

namespace core
{
    FLDRColor::FLDRColor()
    {

    }

    FLDRColor::FLDRColor(uint8 InR, uint8 InG, uint8 InB, uint8 InA):
    R(InR),
    G(InG),
    B(InB),
    A(InA)
    {
    }

    FHDRColor FLDRColor::ToHDRColor() const
    {
        float Denominator = 1 / 255.0f;

        FHDRColor Result = FHDRColor(static_cast<float>(R) * Denominator, static_cast<float>(G) * Denominator, static_cast<float>(B) * Denominator, static_cast<float>(A) * Denominator);

        return Result;
    }
}
