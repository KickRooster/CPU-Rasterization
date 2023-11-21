#include "HDRColor.h"

namespace core
{
    FHDRColor::FHDRColor():
    R(0),
    G(0),
    B(0),
    A(0)
    {
    }

    FHDRColor::FHDRColor(float InR, float InG, float InB, float InA):
    R(InR),
    G(InG),
    B(InB),
    A(InA)
    {
    }

    FHDRColor::FHDRColor(const FHDRColor& Color)
    {
        R = Color.R;
        G = Color.G;
        B = Color.B;
        A = Color.A;
    }

    FHDRColor& FHDRColor::operator=(const FHDRColor& Color)
    {
        R = Color.R;
        G = Color.G;
        B = Color.B;
        A = Color.A;

        return *this;
    }
}
