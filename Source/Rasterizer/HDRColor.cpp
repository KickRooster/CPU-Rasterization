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

    FHDRColor::FHDRColor(const FHDRColor& Color):
    R(Color.R),
    G(Color.G),
    B(Color.B),
    A(Color.A)
    {
    }

    FHDRColor& FHDRColor::operator=(const FHDRColor& Color)
    {
        R = Color.R;
        G = Color.G;
        B = Color.B;
        A = Color.A;

        return *this;
    }

    FHDRColor FHDRColor::operator+(const FHDRColor& Value)
    {
        return FHDRColor(R + Value.R, G + Value.G, B + Value.B, A + Value.A);
    }

    FHDRColor FHDRColor::operator*(float Scale) const
    {
        return  FHDRColor(R * Scale, G * Scale, B * Scale, A * Scale);
    }

    FHDRColor FHDRColor::operator/(float Scale) const
    {
        return  FHDRColor(R / Scale, G / Scale, B / Scale, A / Scale);
    }

    FHDRColor FHDRColor::Red = FHDRColor(1.0, 0, 0, 1.0);
    FHDRColor FHDRColor::Green = FHDRColor(0, 1.0, 0, 1.0);
    FHDRColor FHDRColor::Blue = FHDRColor(0, 0, 1.0, 1.0);
}
