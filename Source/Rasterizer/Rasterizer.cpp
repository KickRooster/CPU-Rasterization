#include "Rasterizer.h"
#include "../Math/MathGlobal.h"
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

    //(0,0)-----------------
    //     |          V0
    //     |         /  \
    //     |        /    \
    //     |       /      \
    //     |      V1-------V2
    //  V0, V1 and V2 have been clipped, they layed in [0, 1].
    void URasterizer::FillUpBottomFlatTriangle(const FVertex& V0, const FVertex& V1, const FVertex& V2, FIrradianceBuffer& IrradianceBuffer) const
    {
        float InvSlope1 = (V1.Position.X - V0.Position.X) / (V1.Position.Y - V0.Position.Y);
        float InvSlope2 = (V2.Position.X - V0.Position.X) / (V2.Position.Y - V0.Position.Y);

        float CurrentX1 = V0.Position.X;
        float CurrentX2 = V0.Position.X;
        float CurrentY = V0.Position.Y;

        int32 V0Y = static_cast<int32>(round(V0.Position.Y));
        int32 V1V2Y = static_cast<int32>(round(V1.Position.Y));

        int32 ScanLineY = V0Y;
        do
        {
            int32 StartX = static_cast<int32>(round(CurrentX1));
            int32 EndX = static_cast<int32>(round(CurrentX2));

            for (int32 i = StartX; i <= EndX; ++i)
            {
                if (!IrradianceBuffer.PixelPostionValid(ScanLineY, i))
                {
                    continue;
                }

                FVector2 Point0 = FVector2(V0.Position.X, V0.Position.Y);
                FVector2 Point1 = FVector2(V1.Position.X, V1.Position.Y);
                FVector2 Point2 = FVector2(V2.Position.X, V2.Position.Y);
                FVector2 PointP = FVector2(CurrentX1 + i, CurrentY);

                float Area012 = GetArea(Point0, Point1, Point2);

                float Area1P2 = GetArea(PointP, Point1, Point2);
                float Point0Weight = Area1P2 / Area012;

                float Area2P0 = GetArea(PointP, Point2, Point0);
                float Point1Weight = Area2P0 = Area012;

                float Area0P1 = GetArea(PointP, Point0, Point1);
                float Point2Weight = Area0P1 / Area012;

                FHDRColor InterpolatedColor = V0.Color * Point0Weight + V1.Color * Point1Weight + V2.Color * Point2Weight;
                IrradianceBuffer.FillUpOnePixel(ScanLineY, i, InterpolatedColor);
            }

            CurrentX1 += InvSlope1;
            CurrentX2 += InvSlope2;
            ++ScanLineY;
            CurrentY += 1.0f;
        }
        while(ScanLineY < V1V2Y);
        ///////////////////////////////////////////

        // float InvSlope1 = (V1.Position.X - V0.Position.X) / (V1.Position.Y - V0.Position.Y);
        // float InvSlope2 = (V2.Position.X - V0.Position.X) / (V2.Position.Y - V0.Position.Y);
        //
        // float CurrentX1 = V0.Position.X;
        // float CurrentX2 = V0.Position.X;
        //
        // int32 V0Y = static_cast<int32>(round(V0.Position.Y));
        // int32 V1V2Y = static_cast<int32>(round(V1.Position.Y));
        //
        // int32 ScanLineY = V0Y;
        // do
        // {
        //     int32 StartX = static_cast<int32>(round(CurrentX1));
        //     int32 EndX = static_cast<int32>(round(CurrentX2));
        //
        //     IrradianceBuffer.FillUpHorizontal(ScanLineY, StartX, EndX, V0.Color);
        //     CurrentX1 += InvSlope1;
        //     CurrentX2 += InvSlope2;
        //
        //     ++ScanLineY;
        // }
        // while(ScanLineY < V1V2Y);
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

            IrradianceBuffer.FillUpHorizontal(ScanLineY, StartX, EndX, V0.Color);
            CurrentX1 -= InvSlope1;
            CurrentX2 -= InvSlope2;

            --ScanLineY;
        }
        while (ScanLineY > V0V1Y);
    }

    void URasterizer::DoStandard(const FTriangle& Triangle, FIrradianceBuffer& IrradianceBuffer) const
    {
        FTriangle SortedTriangle = SortByY(Triangle);

        //  Till now, the original of the coordinate of Triangle is top-left, after viewport transformation.

        //  Check for trivial case of bottom-flat triangle.
        if (Equal(SortedTriangle.V1.Position.Y, SortedTriangle.V2.Position.Y))
        {
            if (SortedTriangle.V1.Position.X <= SortedTriangle.V2.Position.X)
            {
                FillUpBottomFlatTriangle(SortedTriangle.V0, SortedTriangle.V1, SortedTriangle.V2, IrradianceBuffer);
            }
            else
            {
                FillUpBottomFlatTriangle(SortedTriangle.V0, SortedTriangle.V2, SortedTriangle.V1, IrradianceBuffer);
            }
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
            //  Patch a line between two triangles.
            {
                int32 StartX = static_cast<int32>(round(SortedTriangle.V1.Position.X));
                int32 EndX = static_cast<int32>(round(V3.Position.X));
                if (EndX < StartX)
                {
                    std::swap(StartX, EndX);
                }
                IrradianceBuffer.FillUpHorizontal(static_cast<int32>(round(V3.Position.Y)), StartX, EndX, SortedTriangle.V0.Color);
            }
            FillUpTopFlatTriangle(SortedTriangle.V1, V3, SortedTriangle.V2, IrradianceBuffer);
        }
    }

    void URasterizer::RasterizeTriangle(const FTriangle& Triangle, FIrradianceBuffer& IrradianceBuffer) const
    {
        DoStandard(Triangle, IrradianceBuffer);
    }
}
