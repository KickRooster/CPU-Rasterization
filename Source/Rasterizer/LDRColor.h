#pragma once
#include <Core.h>
#include "HDRColor.h"

namespace core
{
    struct FLDRColor
    {
        uint8 R;
        uint8 G;
        uint8 B;
        uint8 A;

        FLDRColor();
        FLDRColor(uint8 InR, uint8 InG, uint8 InB, uint8 InA);
        FHDRColor ToHDRColor() const;
    };
}
