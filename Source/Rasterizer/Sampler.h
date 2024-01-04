#pragma once
#include <Core.h>
#include "../Assets/Texture.h"
#include "../Rasterizer/LDRColor.h"
#include "../Math/Vector2.h"

namespace core
{
    enum EFilterMode : uint8
    {
        FE_Nearest,
        FE_Blinear,
        FE_Trilinear,
    };

    enum EWrappingMode : uint8
    {
        WE_Repeat,
        WE_ClmapToEdge,
    };

    //  Texture coordinates start at (0,0) for the upper left corner of a texture image to (1,1) for the lower right corner of a texture image.
    class USampler
    {
    public:
        USampler();
        FLDRColor Sample(const FTexture& Texture, EFilterMode FilterMode, EWrappingMode WrappingMode, const FVector2& UV);
        ~USampler();
    };
}
