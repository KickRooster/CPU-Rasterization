#include "IrradianceBuffer.h"

#include <algorithm>
#include <cassert>

namespace core
{
    FIrradianceBuffer::FIrradianceBuffer(int32 InWidth, int32 InHeight):
    Width(InWidth),
    Height(InHeight)
    {
        HDRData = new FHDRColor*[Height];
        for (int32 i = 0; i < Height; ++i)
        {
            HDRData[i] = new FHDRColor[Width]();
        }

        LDRData = new FLDRColor*[Height];
        for (int32 i = 0; i < Height; ++i)
        {
            LDRData[i] = new FLDRColor[Width]();
        }

        FlippedLDRRawData = new uint8[Width * Height * sizeof(FLDRColor)];
    }

    void FIrradianceBuffer::Clear(const FHDRColor& Color)
    {
        for (int32 i = 0; i < Height; ++i)
        {
            for (int32 j = 0; j < Width; ++j)
            {
                HDRData[i][j] = Color;
            }
        }
    }

    bool FIrradianceBuffer::PixelPostionValid(int32 Y, int32 X) const
    {
        if (Y < 0 || Y >= Height)
        {
            return false;
        }

        if (X < 0 || X >= Width)
        {
            return false;
        }

        return true;
    }

    void FIrradianceBuffer::FillUpOnePixel(int32 Y, int32 X, const FHDRColor& Color)
    {
        if (Y < 0 || Y >= Height)
        {
            return;
        }

        HDRData[Y][X] = Color;
    }

    void FIrradianceBuffer::ToneMaping()
    {
        for (int32 i = 0; i < Height; ++i)
        {
            for (int32 j = 0; j < Width; ++j)
            {
                LDRData[i][j].R = static_cast<uint8>(HDRData[i][j].R * 255.0f);
                LDRData[i][j].G = static_cast<uint8>(HDRData[i][j].G * 255.0f);
                LDRData[i][j].B = static_cast<uint8>(HDRData[i][j].B * 255.0f);
                LDRData[i][j].A = static_cast<uint8>(HDRData[i][j].A * 255.0f);

                //  XXX:    UI rendering using OpenGL RHI, its texture coordinates start at (0,0) for the lower left corner of a texture image to (1,1) for the upper right corner of a texture image.
                //          Our irradiance buffer coordinates start at (0,0) for the upper left corner to (1,1) for the lower right.
                //          So we should flip our irradiance buffer for rendering UI.
                FlippedLDRRawData[((Height - i - 1) * Width + j) * sizeof(FLDRColor) + 0] = LDRData[i][j].R;
                FlippedLDRRawData[((Height - i - 1) * Width + j) * sizeof(FLDRColor) + 1] = LDRData[i][j].G;
                FlippedLDRRawData[((Height - i - 1) * Width + j) * sizeof(FLDRColor) + 2] = LDRData[i][j].B;
                FlippedLDRRawData[((Height - i - 1) * Width + j) * sizeof(FLDRColor) + 3] = LDRData[i][j].A;
            }
        }
    }

    uint8 * FIrradianceBuffer::GetRawLDRData() const
    {
        return FlippedLDRRawData;
    }

    FIrradianceBuffer::~FIrradianceBuffer()
    {
        for (int32 i = 0; i < Height; ++i)
        {
            delete[] HDRData[i];
            delete[] LDRData[i];
        }

        delete[] HDRData;
        delete[] LDRData;
        delete[] FlippedLDRRawData;
    }
}
