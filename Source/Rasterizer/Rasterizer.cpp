#include "Rasterizer.h"

#include <algorithm>

namespace core
{
    //  Sort the three vertices by y-coordinate ascending so v0 is the topmost vertice
    FTriangle URasterizer::SortByY(const FTriangle& Triangle) const
    {
        FVertex Vertices[3] = {Triangle.V0, Triangle.V1, Triangle.V2};
        for (int32 i = 1; i < 3; ++i)
        {
            FVertex Key = Vertices[i];
            int32 j = i - 1;
            while(j >= 0 && Vertices[j].Position.Y > Key.Position.Y)
            {
                Vertices[j + 1] = Vertices[j];
                --j;
            }
            Vertices[j + 1] = Key;
        }

        FTriangle SortedTriangle = FTriangle(Vertices[0], Vertices[1], Vertices[2]);

        return SortedTriangle;
    }

    void URasterizer::FillUpBottomFlatTriangle(const FVertex& V0, const FVertex& V1, const FVertex& V2, FIrradianceBuffer& IrradianceBuffer) const
    {
        float InvSlope1 = (V1.Position.X - V0.Position.X) / (V1.Position.Y - V0.Position.Y);
        float InvSlope2 = (V2.Position.X - V0.Position.X) / (V2.Position.Y - V0.Position.Y);

        float CurrentX1 = V0.Position.X;
        float CurrentX2 = V0.Position.X;

        int32 V0Y = static_cast<int32>(round(V0.Position.Y));
        int32 V1V2Y = static_cast<int32>(round(V1.Position.Y));

        int32 ScanLineY = V0Y;
        do
        {
            int32 StartX = static_cast<int32>(round(CurrentX1));
            int32 EndX = static_cast<int32>(round(CurrentX2));

            if (StartX > EndX)
            {
                std::swap(StartX, EndX);
            }

            //  XXX:    Hard code 0, 1, 0, 1.
            IrradianceBuffer.FillUpHorizontal(ScanLineY, StartX, EndX, FHDRColor(0, 1, 0, 1));
            CurrentX1 += InvSlope1;
            CurrentX2 += InvSlope2;

            ++ScanLineY;
        }
        while(ScanLineY < V1V2Y);

        // for (int32 ScanLineY = V0Y; ScanLineY < V1V2Y; ++ScanLineY)
        // {
        //     int32 StartX = static_cast<int32>(round(CurrentX1));
        //     int32 EndX = static_cast<int32>(round(CurrentX2));
        //
        //     if (StartX > EndX)
        //     {
        //         std::swap(StartX, EndX);
        //     }
        //
        //     //  XXX:    Hard code 0, 1, 0, 1.
        //     IrradianceBuffer.FillUpHorizontal(ScanLineY, StartX, EndX, FHDRColor(0, 1, 0, 1));
        //     CurrentX1 += InvSlope1;
        //     CurrentX2 += InvSlope2;
        // }
    }

    void URasterizer::FillUpTopFlatTriangle(const FVertex& V0, const FVertex& V1, const FVertex& V2, FIrradianceBuffer& IrradianceBuffer) const
    {
        float InvSlope1 = (V2.Position.X - V0.Position.X) / (V2.Position.Y - V0.Position.Y);
        float InvSlope2 = (V2.Position.X - V1.Position.X) / (V2.Position.Y - V1.Position.Y);

        float CurrentX1 = V2.Position.X;
        float CurrentX2 = V2.Position.X;

        int32 V2Y = static_cast<int32>(round(V2.Position.Y));
        int32 V0V1Y = static_cast<int32>(round(V0.Position.Y));

        int32 ScanLineY = V2Y;
        do
        {
            int32 StartX = static_cast<int32>(round(CurrentX1));
            int32 EndX = static_cast<int32>(round(CurrentX2));

            if (StartX > EndX)
            {
                std::swap(StartX, EndX);
            }

            //  XXX:    Hard code 0, 0, 1, 1
            IrradianceBuffer.FillUpHorizontal(ScanLineY, StartX, EndX, FHDRColor(0, 0, 1, 1));
            CurrentX1 -= InvSlope1;
            CurrentX2 -= InvSlope2;

            --ScanLineY;
        }
        while (ScanLineY > V0V1Y);

        // for (int32 ScanLineY = V2Y; ScanLineY > V0V1Y; --ScanLineY)
        // {
        //     int32 StartX = static_cast<int32>(round(CurrentX1));
        //     int32 EndX = static_cast<int32>(round(CurrentX2));
        //
        //     if (StartX > EndX)
        //     {
        //         std::swap(StartX, EndX);
        //     }
        //
        //     //  XXX:    Hard code 0, 0, 1, 1
        //     IrradianceBuffer.FillUpHorizontal(ScanLineY, StartX, EndX, FHDRColor(0, 0, 1, 1));
        //     CurrentX1 -= InvSlope1;
        //     CurrentX2 -= InvSlope2;
        // }
    }

    void URasterizer::DoStandard(const FTriangle& Triangle, FIrradianceBuffer& IrradianceBuffer) const
    {
        FTriangle SortedTriangle = SortByY(Triangle);

        //  Check for trivial case of bottom-flat triangle.
        if (Equal(SortedTriangle.V1.Position.Y, SortedTriangle.V2.Position.Y))
        {
            FillUpBottomFlatTriangle(SortedTriangle.V0, SortedTriangle.V1, SortedTriangle.V2, IrradianceBuffer);
        }
        //  Check for trivial case of top-flat triangle.
        else if (Equal(SortedTriangle.V0.Position.Y, SortedTriangle.V1.Position.Y))
        {
            FillUpTopFlatTriangle(SortedTriangle.V0, SortedTriangle.V1, SortedTriangle.V2, IrradianceBuffer);
        }
        //  Gneral case - split the triangle in a topflat and bottom-flat one.
        else
        {
            FVertex V3 = FVertex(
                SortedTriangle.V0.Position.X + (SortedTriangle.V1.Position.Y - SortedTriangle.V0.Position.Y) / (SortedTriangle.V2.Position.Y - SortedTriangle.V0.Position.Y) * (SortedTriangle.V2.Position.X - SortedTriangle.V0.Position.X),
                SortedTriangle.V1.Position.Y,
                SortedTriangle.V1.Position.Z,   //  TODO:   Process 2d point, discard z component.
                1.0f);
            FillUpBottomFlatTriangle(SortedTriangle.V0, SortedTriangle.V1, V3, IrradianceBuffer);
            FillUpTopFlatTriangle(SortedTriangle.V1, V3, SortedTriangle.V2, IrradianceBuffer);
        }
    }

    void URasterizer::RasterizeTriangle(const FTriangle& Triangle, FIrradianceBuffer& IrradianceBuffer) const
    {
        DoStandard(Triangle, IrradianceBuffer);
    }
}
