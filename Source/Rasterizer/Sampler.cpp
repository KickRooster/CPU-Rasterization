#include "Sampler.h"

namespace core
{
    USampler::USampler()
    {

    }

    //  TODO:   Min and Mag filter.
    FLDRColor USampler::Sample(const FTexture& Texture, EFilterMode FilterMode, EWrappingMode WrappingMode, const FVector2& UV)
    {
        FVector2 ProcessedUV = UV;

        if (WrappingMode == WE_Repeat)
        {
            while (ProcessedUV.X < 0)
            {
                ProcessedUV.X += 1.0f;
            }

            while (ProcessedUV.Y < 0)
            {
                ProcessedUV.Y += 1.0f;
            }

            while (ProcessedUV.X > 0)
            {
                ProcessedUV.X -= 1.0f;
            }

            while (ProcessedUV.Y > 0)
            {
                ProcessedUV.Y -= 1.0f;
            }
        }
        else if (WrappingMode == WE_ClmapToEdge)
        {
            if (ProcessedUV.X < 0)
            {
                ProcessedUV.X = 0;
            }

            if (ProcessedUV.Y < 0)
            {
                ProcessedUV.Y = 0;
            }

            if (ProcessedUV.X > 1.0f)
            {
                ProcessedUV.X = 1.0f;
            }

            if (ProcessedUV.Y > 1.0f)
            {
                ProcessedUV.Y = 1.0f;
            }
        }

        ProcessedUV.X *= static_cast<float>(Texture.Width);
        ProcessedUV.Y *= static_cast<float>(Texture.Height);

        //  Set alpha to 255 as default.
        FLDRColor Result = FLDRColor(0, 0, 0, 255);

        if (FilterMode == FE_Nearest)
        {
            int32 U = static_cast<int32>(roundf(ProcessedUV.X));
            int32 V = static_cast<int32>(roundf(ProcessedUV.Y));

            int32 Offset = V * Texture.Width * Texture.ComponentCount;
            Offset += U * Texture.ComponentCount;

            Result.R = Texture.Data[Offset];
            Result.G = Texture.Data[Offset + 1];
            Result.B = Texture.Data[Offset + 2];

            if (Texture.ComponentCount == 4)
            {
                Result.A = Texture.Data[Offset + 3];
            }
        }
        else if (FilterMode == FE_Blinear)
        {

        }
        else //if (FilterMode == FE_Trilinear)
        {
            //  TODO:   Mipmap generated
        }

        return Result;
    }

    USampler::~USampler()
    {

    }
}
