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
    };
}
