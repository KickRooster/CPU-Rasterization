#pragma once
#include "IAsset.h"

namespace core
{
    struct FTexture
    {
        int32 Width;
        int32 Height;
        int32 ComponentCount;

        uint8* Data;
    };
}
