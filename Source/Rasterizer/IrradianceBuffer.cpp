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

        LDRRawData = new uint8[Width * Height * sizeof(FLDRColor)];
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

    void FIrradianceBuffer::FillUpHorizontal(int32 Y, int32 StartX, int32 EndX, const FHDRColor& Color)
    {
        StartX = std::max(0, StartX);
        EndX = std::min(EndX, Width - 1);

        for (int32 i = StartX; i <= EndX; ++i)
        {
            HDRData[Y][i] = Color;
        }
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

                LDRRawData[(i * Width + j) * sizeof(FLDRColor) + 0] = LDRData[i][j].R;
                LDRRawData[(i * Width + j) * sizeof(FLDRColor) + 1] = LDRData[i][j].G;
                LDRRawData[(i * Width + j) * sizeof(FLDRColor) + 2] = LDRData[i][j].B;
                LDRRawData[(i * Width + j) * sizeof(FLDRColor) + 3] = LDRData[i][j].A;
            }
        }
    }

    uint8 * FIrradianceBuffer::GetRawLDRData() const
    {
        return LDRRawData;
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
        delete[] LDRRawData;
    }
}
