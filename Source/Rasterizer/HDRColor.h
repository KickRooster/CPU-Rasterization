#pragma once
#include <Core.h>

namespace core
{
    struct FHDRColor
    {
        float R;
        float G;
        float B;
        float A;

        FHDRColor();
        FHDRColor(float InR, float InG, float InB, float InA);
        FHDRColor(const FHDRColor& Color);
        FHDRColor& operator =(const FHDRColor& Color);
        FHDRColor operator +(const FHDRColor& Value);
        FHDRColor operator * (float Scale) const;
        FHDRColor operator / (float Scale) const;
        static FHDRColor Red;
        static FHDRColor Green;
        static FHDRColor Blue;
    };
}
